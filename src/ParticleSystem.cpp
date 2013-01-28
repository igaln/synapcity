#include "ParticleSystem.h"
#include "cinder/app/AppBasic.h"

float ParticleSystem::dampingForce = 0.1;
float ParticleSystem::timeStep = 0.2;
ParticleSystem::ParticleSystem()
{
	
}

void ParticleSystem::setup(int width, int height, int k) {
	this->width = width;
	this->height = height;
	this->k = k;
	binSize = 1 << k;
	xBins = (int) ceilf((float) width / (float) binSize);
	yBins = (int) ceilf((float) height / (float) binSize);
	bins.resize(xBins * yBins);
	
}

void ParticleSystem::setTimeStep(float timeStep) {
	ParticleSystem::timeStep = timeStep;
}

void ParticleSystem::add(Particle* particle) {
	particles.push_back(particle);
}

unsigned ParticleSystem::size() const {
	return particles.size();
}

Particle* ParticleSystem::operator[](unsigned i) {
	return particles[i];
}

vector<Particle*> ParticleSystem::getNeighbors(Particle& particle, float radius) {
	return getNeighbors(particle.x, particle.y, radius);
	
}

vector<Particle*> ParticleSystem::getNeighbors(float x, float y, float radius) {
	vector<Particle*> region = getRegion(
		(int) (x - radius),
		(int) (y - radius),
		(int) (x + radius),
		(int) (y + radius));
	vector<Particle*> neighbors;
	int n = region.size();
	float xd, yd, rsq, maxrsq;
	maxrsq = radius * radius;
	for(int i = 0; i < n; i++) {
		Particle& cur = *region[i];
		xd = cur.x - x;
		yd = cur.y - y;
		rsq = xd * xd + yd * yd;
		if(rsq < maxrsq)
			neighbors.push_back(region[i]);
	}
	return neighbors;
}

vector<Particle*> ParticleSystem::getRegion(unsigned minX, unsigned minY, unsigned maxX, unsigned maxY) {
	vector<Particle*> region;
	back_insert_iterator< vector<Particle*> > back = back_inserter(region);
	unsigned minXBin = minX >> k;
	unsigned maxXBin = maxX >> k;
	unsigned minYBin = minY >> k;
	unsigned maxYBin = maxY >> k;
	maxXBin++;
	maxYBin++;
	if(maxXBin > xBins)
		maxXBin = xBins;
	if(maxYBin > yBins)
		maxYBin = yBins;
	for(int y = minYBin; y < maxYBin; y++) {
		for(int x = minXBin; x < maxXBin; x++) {
			vector<Particle*>& cur = bins[y * xBins + x];
			copy(cur.begin(), cur.end(), back);
		}
	}
	return region;
}

void ParticleSystem::setupForces() {
	int n = bins.size();
	for(int i = 0; i < n; i++) {
		bins[i].clear();
	}
	forces.clear();
	forceLines.clear();
	n = particles.size();
	unsigned xBin, yBin, bin;
	for(int i = 0; i < n; i++) {
		Particle* cur = particles[i];
		cur->resetForce();
		xBin = ((unsigned) cur->x) >> k;
		yBin = ((unsigned) cur->y) >> k;
		bin = yBin * xBins + xBin;
		if(xBin < xBins && yBin < yBins)
			bins[bin].push_back(cur);
	}
}


void ParticleSystem::addRepulsionForce(const Particle& particle, float radius, float scale) {
	addRepulsionForce(particle.x, particle.y, radius, scale);
}

void ParticleSystem::addRepulsionForce(float x, float y, float radius, float scale) {
	addForce(x, y, radius, scale, false, 0);
}

void ParticleSystem::addAttractionForce(const Particle& particle, float radius, float scale) {
	addAttractionForce(particle.x, particle.y, radius, scale);
}

void ParticleSystem::addAttractionForce(float x, float y, float radius, float scale) {
	addForce(x, y, radius, -scale, false, 0);
}

void ParticleSystem::addForce(const Particle& particle, float radius, float scale) {
	addForce(particle.x, particle.y, radius, -scale, false, 0);
}
void ParticleSystem::addForce(float targetX, float targetY, float radius, float scale, bool clusterForce, int clusterID){
	force f;
	f.x =targetX;
	f.y = targetY;
	f.radius = radius;
	f.scale = scale;
	f.clusterForce = clusterForce;
	f.clusterID= clusterID;
	forces.push_back(f);
}
void ParticleSystem::applyForces()
{
	forceIter = forces.begin();
	while (forceIter!=forces.end()) {
		applyForce((*forceIter).x, (*forceIter).y, (*forceIter).radius, (*forceIter).scale, (*forceIter).clusterForce, (*forceIter).clusterID);
		++forceIter;
	}
	
}
void ParticleSystem::applyForce(float targetX, float targetY, float radius, float scale, bool clusterForce = false, int clusterID = 0) {
	float minX = targetX - radius;
	float minY = targetY - radius;
	float maxX = targetX + radius;
	float maxY = targetY + radius;
	if(minX < 0)
		minX = 0;
	if(minY < 0)
		minY = 0;
	unsigned minXBin = ((unsigned) minX) >> k;
	unsigned minYBin = ((unsigned) minY) >> k;
	unsigned maxXBin = ((unsigned) maxX) >> k;
	unsigned maxYBin = ((unsigned) maxY) >> k;
	maxXBin++;
	maxYBin++;
	if(maxXBin > xBins)
		maxXBin = xBins;
	if(maxYBin > yBins)
		maxYBin = yBins;
	float xd, yd, length, maxrsq;
	#ifdef USE_INVSQRT
	float xhalf;
	int lengthi;
	#else
	float effect;
	#endif
	maxrsq = radius * radius;
	int x,y;
	for(y = minYBin; y < maxYBin; y++) {
		for(x = minXBin; x < maxXBin; x++) {
			vector<Particle*>& curBin = bins[y * xBins + x];
			int n = curBin.size();
			for(int i = 0; i < n; i++) {
				Particle& curParticle = *(curBin[i]);

				if ((clusterForce && curParticle.clusterID == clusterID) || !clusterForce) {
					//if (!clusterForce) {
//						cout <<  "NOT ONE" << endl;
//					}
					
//				As suggested by Andrew Bell
//				if(curParticle.x > minX && curParticle.x < maxX &&
//					curParticle.y > minY && curParticle.y < maxY) {
					xd = curParticle.x - targetX;
					yd = curParticle.y - targetY;
					length = xd * xd + yd * yd;
					if(length > 0 && length < maxrsq) {
						#ifdef DRAW_FORCES
						if (length < 400) {
							fLine l;
							l.xA = targetX;
							l.yA = targetY;
							l.xB = curParticle.x;
							l.yB = curParticle.y;
							if(curParticle.live)
							l.color = curParticle.color;
							else {
								l.color = ColorA(1.0f,1.0f,1.0f,0.6f);

							}

							forceLines.push_back(l);
						}
						#endif
						#ifdef USE_INVSQRT
							xhalf = 0.5f * length;
							lengthi = *(int*) &length;
							lengthi = 0x5f3759df - (lengthi >> 1);
							length = *(float*) &lengthi;
							length *= 1.5f - xhalf * length * length;
							xd *= length;
							yd *= length;
							length *= radius;
							length = 1 / length;
							length = (1 - length);
							length *= scale;
							xd *= length;
							yd *= length;
							curParticle.xf += xd;
							curParticle.yf += yd;
						#else
							length = sqrtf(length);
							xd /= length;
							yd /= length;
							effect = (1 - (length / radius)) * scale;
							curParticle.xf += xd * effect;
							curParticle.yf += yd * effect;
						#endif
					}
				}
			}
		}
	}
}

void ParticleSystem::addClusterRepulsionForce(const Particle& particle, float radius, float scale, int clusterID)
{
	addClusterRepulsionForce(particle.x, particle.y, radius, scale, clusterID);
}
void ParticleSystem::addClusterRepulsionForce(float x, float y, float radius, float scale, int clusterID)
{
	addForce(x, y, radius, scale, true, clusterID);
}
void ParticleSystem::addClusterAttractionForce(const Particle& particle, float radius, float scale, int clusterID)
{
	addClusterAttractionForce(particle.x, particle.y, radius, scale, clusterID);
}
void ParticleSystem::addClusterAttractionForce(float x, float y, float radius, float scale, int clusterID)
{
	addForce(x, y, radius, -scale, true, clusterID);
}

void ParticleSystem::update() {
	
	int n = particles.size();
	for(int i = 0; i < n; i++){
		particles[i]->addDampingForce(dampingForce);
		particles[i]->updatePosition(timeStep);
		//particles[i]->bounceOffWalls(0, 0, ci::app::App::get()->getWindowWidth(), ci::app::App::get()->getWindowHeight());
		
	}
}

void ParticleSystem::draw() {
	
    /*
	forceLineIter = forceLines.begin();
	glBegin(GL_LINES);
	while (forceLineIter!=forceLines.end()) {
		(*forceLineIter).color.a = 0.01;
		
		//glColor4f((*forceLineIter).color);
	
		if(live) {
			glColor4f((*forceLineIter).color);
		}
		else {
			glColor4f(1.0f,1.0f,1.0f,0.05f);
		}
		
        
        //glColor4f(1.0f,1.0f,1.0f,0.05f);
        glColor4f((*forceLineIter).color);
		
		
		glVertex2f((*forceLineIter).xA, (*forceLineIter).yA);
		glVertex2f((*forceLineIter).xB, (*forceLineIter).yB);
		
		++forceLineIter;
	}
	glEnd();
    */

	int n = particles.size();
	glBegin(GL_POINTS);
	for(int i = 0; i < n; i++)
		particles[i]->draw();
	glEnd();
	
}
