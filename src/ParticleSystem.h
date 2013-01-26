/*
 *  clusterSystem.h
 *  synapcityVizV3
 *
 *  Created by Igal Nassima on 10/31/10.
 *
 *	K-bin particle system.
 *	Handles everything adding,removing particles, forces of repulsion, attraction
 */


#pragma once
#include "Particle.h"
#include "cinder/Color.h"
//#include "cinder/gl/gl.h"
//#include "cinder/gl/Vbo.h"
#include <vector>
//#include <list>

#define DRAW_FORCES
#define USE_INVSQRT
struct force {
	float x;
	float y;
	float radius;
	float scale;
	bool clusterForce;
	int clusterID;
};
struct fLine {
	float xA;
	float yA;
	float xB;
	float yB;
	ColorA color;
};
class ParticleSystem {
protected:
	vector<Particle*> particles;
	vector< vector<Particle*> > bins;
	vector< force > forces;
	vector< force >::iterator forceIter;
	vector< fLine > forceLines;
	vector< fLine >::iterator forceLineIter;
	int width, height, k, xBins, yBins, binSize;
	
	//gl::VboMesh vboParticles , vboLines;
	//gl::VboMesh::Layout layoutParticles, layoutLines;

public:
	static float dampingForce;
	static float timeStep;
	//bool live;
	ParticleSystem();

	void setup(int width, int height, int k);
	void setTimeStep(float timeStep);

	void add(Particle* particle);
	vector<Particle*> getNeighbors(Particle& particle, float radius);
	vector<Particle*> getNeighbors(float x, float y, float radius);
	vector<Particle*> getRegion(unsigned minX, unsigned minY, unsigned maxX, unsigned maxY);
	unsigned size() const;
	Particle* operator[](unsigned i);

	void setupForces();
	//
	void addRepulsionForce(const Particle& particle, float radius, float scale);
	void addRepulsionForce(float x, float y, float radius, float scale);
	void addAttractionForce(const Particle& particle, float radius, float scale);
	void addAttractionForce(float x, float y, float radius, float scale);
	void addForce(const Particle& particle, float radius, float scale);
	void addForce(float x, float y, float radius, float scale, bool clusterForce = false, int clusterID = 0);
	void applyForce(float x, float y, float radius, float scale, bool clusterForce, int clusterID);
	void applyForces();
	//
	void addClusterRepulsionForce(const Particle& particle, float radius, float scale, int clusterID);
	void addClusterRepulsionForce(float x, float y, float radius, float scale, int clusterID);
	void addClusterAttractionForce(const Particle& particle, float radius, float scale, int clusterID);
	void addClusterAttractionForce(float x, float y, float radius, float scale, int clusterID);
	
	void update();

	void draw();
	void drawForces();
	
};

inline float InvSqrt(float x){
	float xhalf = 0.5f * x;
	int i = *(int*)&x; // store floating-point bits in integer
	i = 0x5f3759d5 - (i >> 1); // initial guess for Newton's method
	x = *(float*)&i; // convert new bits into float
	x = x*(1.5f - xhalf*x*x); // One round of Newton's method
	return x;
}

