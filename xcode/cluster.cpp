/*
 *  cluster.cpp
 *  synapcityVizV3
 *
 *  Created by Nikolas Psaroudakis on 10/31/10.
 *  Edited by Igal Nassima
 *
 */

#include "cluster.h"
#include "cinder/Rand.h"
#include "cinder/Color.h"
#include "cinder/app/AppBasic.h"

using namespace ci;
bool cluster::drawLabel = true;
cluster::cluster(ParticleSystem* _pSystem, Surface _surf, int _clusterID, int _nParticles):
pSystem(_pSystem), surf(_surf), clusterID(_clusterID), nParticles(_nParticles)
{
	setup();
}
void cluster::setup()
{
	currentJsonHistoryLine = 0;
	centerAttraction = 50;
	float padding = 10;
	float maxVelocity = .05;
	width = app::App::get()->getWindowWidth();
	height =app::App::get()->getWindowHeight();
	for (int i=0; i<nParticles; i++) {
		ColorA myColor = surf.getPixel( Vec2i(Rand::randInt(0, 600 ),Rand::randInt(0, 600 )));
		float x = Rand::randFloat(padding, width - padding);
		float y = Rand::randFloat(padding, height - padding);
		float xv = Rand::randFloat(-maxVelocity, maxVelocity);
		float yv = Rand::randFloat(-maxVelocity, maxVelocity);
		Particle* p = new Particle(x,y,xv,yv,myColor, clusterID);
		pSystem->add(p);
	}
	theLabel = Label(pos+Vec2i(-30,100));
	aTimer = ci::Timer();
	
	particleNeighborhood = 20;
	live = true;
}
void cluster::setPosition(ci::Vec2i pos)
{
	this->pos = pos;
	theLabel = Label(pos+Vec2i(-35,100));
}
void cluster::update()
{
	
	//pSystem->addClusterAttractionForce(pos.x, pos.y, ci::app::App::get()->getWindowWidth(), centerAttraction, true, clusterID);
	
}
void cluster::draw()
{
	//cout << "cluster is " << live << endl;
	//pSystem->live = live;
	//Randomly updating the equalizer in the label
	//To be removed
	if (aTimer.isStopped()) {
		aTimer.start();
	} else if (aTimer.getSeconds()>=1){
		for (int i=0; i<theLabel.equalizer.nLines; i++) {
			
			theLabel.setData(i, Rand::randFloat());
			
		}
		aTimer.stop();
		aTimer = ci::Timer();
	}
	
	if (drawLabel) {
		theLabel.draw();
	}
}