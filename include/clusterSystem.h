/*
 *  clusterSystem.h
 *  synapcityVizV3
 *
 *  Created by Nikolas Psaroudakis on 10/31/10.
 *  Edited by Igal Nassima
 *
 *	Cluster system organizes the particle system into clusters for each headset
 *	It is able to handle how many clusters (headsets) we are using for graphics, their positions and history vs live modes.
 *	It inializes other parameters such as number of particles, positions of clusters, urls, images for giving color varioations to particle systems,
 *	initial forces, 
 *
 *	The class also parses Attention and Meditation from JSON EEG data and converts it into Attraction and Repulstion forces 
 *  and applies it to Particle systems.
 *
 *	
 *
 */

#pragma once
#include <vector>
#include <string>
#include "cinder/Vector.h"
#include "cinder/Perlin.h"
#include "ParticleSystem.h"
#include "cluster.h"
#include "jsonRead.h"

using namespace ci;
using namespace std;
class clusterSystem {
public:
	clusterSystem(ParticleSystem *_pSystem, int _numClusters, int _numClusterParticles);
	void setup();
	
	//apply all the forces to particle systems
	void update();
	
	// set positions of all clusters (headsets)
	void setPositions();
	// set images for color distributions
	void setImages();
	// draw clusters
	void draw();
	// Perlin noise setup for motion in the particles
	Vec2i getNoiseOffset(Vec2i pos,float time);
	/* 
	 * All the data is being taken from JSON and stored in each cluster here
	 * Checks if a cluster in history mode, or live
	 * takes the data, setups the forces and update Label Data
	 * the Meditation and Attention is converted to Repulsion and attraction forces
	 */
	void getAndApplyData();	
	// read settings from the XML file
	void readSettings();
	
	int numClusters;
	int numClusterParticles;
	ParticleSystem *pSystem;
	vector <cluster*> clusters;
	vector <Vec2i> positions;
	vector <string> names;
	vector <string> artwork;
	vector <string> images;
	int particleNeighborhood;
	int particleRepulsion;
	Vec2i mousePos;
	
	string liveURL;
	string historyURLpartA;
	string historyURLpartB;
	
	jsonRead liveReader;
	vector<jsonRead> historyReaders;
	std::vector<Color> linecolors;
private:
	Perlin mPerlin;
	double mLastTimerFire;
};