/*
 *  cluster.h
 *  synapcityVizV3
 *
 *  Created by Nikolas Psaroudakis on 10/31/10.
 *	Edited by Igal Nassima
 *  
 *	Each clustersystem is formed of clusters that define properties for particles that belong to a headset.
 *	The Cluster class keeps track of particle system properties including
 *	Attraction, Repulsion, Width, Height, Position, and History data
 *
 */

#pragma once
#include "cinder/Vector.h"
#include "cinder/Surface.h"
#include "ParticleSystem.h"
#include "cinder/Timer.h"
#include "Label.h"
#include "jsonRead.h"

class cluster {
public:
	cluster(ParticleSystem* _pSystem, ci::Surface _surf, int _clusterID, int _nParticles);
	void setup();
	void update();
	void draw();
	void setPosition(ci::Vec2i pos);
	ParticleSystem* pSystem;
	ci::Surface surf;
	int clusterID;
	int nParticles;
	ci::Vec2i pos;
	float width;
	float height;
	float centerAttraction;
	static bool drawLabel;
	Label theLabel;
	float repulsionTime;
	float repulsionHeight;
	float repulsionWidth;
	int particleRepulsion;
	int particleNeighborhood;
	
	//todo: if not live go into history mode.
	bool live;

	int currentJsonHistoryLine;
	
	vector<eegData> datadead;
	
private:
	Timer aTimer;
};