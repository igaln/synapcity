/*
 *  clusterSystem.cpp
 *  synapcityVizV3
 *
 *  Created by Nikolas Psaroudakis on 10/31/10.
 *  Edited by Igal Nassima
 *
 */

#include "clusterSystem.h"
#include "cinder/app/AppBasic.h"
#include "cinder/app/App.h"
#include "cinder/Utilities.h"
#include "cinder/ImageIo.h"
#include "cinder/Surface.h"
#include "cinder/Rand.h"
#include "cinder/Xml.h"
#include <sstream>
#include <string>
#include <boost/algorithm/string.hpp>

#include "json.h"
using namespace std;

typedef std::map<std::string, std::string> TStrStrMap;

clusterSystem::clusterSystem(ParticleSystem *_pSystem, int _numClusters, int _numClusterParticles):
pSystem(_pSystem), numClusters(_numClusters), numClusterParticles(_numClusterParticles)
{
	this->setup();
	
}
void clusterSystem::readSettings()


{
	
	filebuf fb;
	fb.open (ci::app::getResourcePath("settings.xml").c_str(),ios::in);
	istream is(&fb);
	XmlDocument doc( is );
	
	XmlElement myRoot = doc.rootNode();
	XmlElement live = myRoot.findChild("live");
	myRoot = doc.rootNode();
	XmlElement history = myRoot.findChild("history");
	XmlElement art = myRoot.findChild("artwork");
	XmlElement bars = myRoot.findChild("bars");
	
	std::vector<XmlElement> children = bars.children();
	
	
	for (int i = 0; i < children.size(); i++)
		
	{
		
		std::map<std::string, std::string> bcolors = children[i].attributes();
		
		TStrStrMap::iterator k;
		int r,g,b;
		
		for(k = bcolors.begin(); k!=bcolors.end(); ++k)
		{
			
			
			string temp = k->second;
			if(k->first == "r") {
				r = atoi(temp.c_str());
			}
			else if(k->first == "g") {
				g = atoi(temp.c_str());
			}
			else if(k->first == "b") {
				b = atoi(temp.c_str());
			}
			
						
		}
		cout << r << " " << g << " " << b << endl;
		linecolors.push_back(ci::Color(r,g,b));

		
		
	}
		
		
	
	std::map<std::string, std::string> attr = live.attributes();
	std::map<std::string, std::string> attr2 = history.attributes();
	std::map<std::string, std::string> attr3 = art.attributes();
	
	TStrStrMap::iterator p;
		// Show key
	for(p = attr.begin(); p!=attr.end(); ++p)
	{
		std::string strKey;
		//strValue = s;
		strKey= p->second;
		std::cout << p->second << " " << p->first << std::endl;
		liveURL = p->second;
		
	}
	
	TStrStrMap::iterator p2;
	// Show key
	for(p2 = attr2.begin(); p2!=attr2.end(); ++p2)
	{
		if(p2->first == "data") {
			//std::cout << p2->second << " " << p2->first << std::endl
			historyURLpartB = p2->second;
		}
		else {
			historyURLpartA	= p2->second;
		}
		
	}
	
	for(p2 = attr3.begin(); p2!=attr3.end(); ++p2)
	{
				cout << p2->second << endl;
		
		artwork.push_back(p2->second);
			
		
	}

		
	fb.close();
	
	
	
	/*
	string path = "settings.json";
	ifstream infile(path.c_str());
	string mRes((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
	Json::Value mRoot;
	Json::Reader mReader;
	
	bool wasSuccessfull = mReader.parse(mRes,mRoot);
	Json::Value rootOut = mRoot["settings"];
	
	
	liveURL = rootOut.get("liveURL", "localhost").asString();
	//cout << "LIVE URL " << liveURL;
	Json::Value historyData = rootOut["historyURL"];
	historyURLpartA = historyData.get("partA", "ppppA").asString();
	historyURLpartB = historyData.get("partB", "ppppB").asString();
	
	infile.close();
	 */
}
void clusterSystem::setup()
{
	readSettings();
	cout << "LIVE: " << liveURL << endl;
	cout << "HISTORY: " << historyURLpartA << endl;
	cout << "HISTORY: " << historyURLpartB << endl;
	
	mLastTimerFire = 0;
	particleNeighborhood = 50;
	particleRepulsion =  4;
	this->setPositions();
	this->setImages();
	for (int i=0; i<numClusters; i++) {
		Surface mySurface = loadImage(ci::app::App::get()->getResourcePath(images[i]));
		cluster* c = new cluster(pSystem, mySurface, i, numClusterParticles);
		
		c->setPosition(positions[i]);
		c->particleRepulsion = Rand::randInt(12,12);
		
		c->centerAttraction = Rand::randInt(10,10);
		Rand::randomize ();
		//setup variables
		c->repulsionTime = Rand::randFloat(0.01,0.5);
		Rand::randomize ();
		c->repulsionWidth = Rand::randInt(100,520);
		Rand::randomize ();
		c->repulsionHeight = Rand::randInt(100,520);
		
		clusters.push_back(c);
	}
	mPerlin = Perlin();
	//comment this out to go back to reading the file for the live URL
	//string liveURL = "http://strp.stichting-neee.nl:6080/buffer/1/expiration/5/readings.json";  // http://synapserver.dyndns.org:1234/buffer/1/expiration/5/readings.json";
	// strp.stichting-neee.nl:6080/buffer/1/expiration/5/readings.json
	cout << ")))))))))))))))))))))))))))) " << liveURL << endl;
	liveReader = jsonRead(liveURL);
	//liveReader.setURL(URL);
	liveReader.threadedRead();
	//Create history readers
	for (int i=0; i<numClusters; i++) {
		stringstream url;
		url<< historyURLpartA << i+1 << historyURLpartB;
		//url<< "http://synapserver.dyndns.org:1234/artworks/" << i+1 << "/readings.json";
		jsonRead h = jsonRead(url.str());
		cout << url.str() << endl;
		historyReaders.push_back(h);
		
	}
    
	//History reader threader read;
	for (int i=0; i< numClusters; i++) {
		historyReaders[i].threadedRead();
	}
	

	names.push_back("Peter Holzkorn");
	names.push_back("Nikolas Psaroudakis");
	names.push_back("Michael Zick Doherty");
	names.push_back("Barak Harel");
	names.push_back("Mathias Dahlstrom");
	names.push_back("Eyal Fried");
	names.push_back("Kathrine Worel");
	names.push_back("Luther Thei");
	names.push_back("COMMUNAL MIND");
	names.push_back("Igal Nassima");
	
	//"Target", "Lost", "Wet", "PainStation", "Proun", "Protoquatro", "Sissy", "Physiognomic Scrutinizer", "tVisionarium", "Hostage"
	
		
}
void clusterSystem::setPositions()
{
	//updated to push back to single large position
    positions.push_back(ci::Vec2i(550, 480));
    
    
    /*
    positions.push_back(ci::Vec2i(306, 180));
    positions.push_back(ci::Vec2i(510, 180));
    positions.push_back(ci::Vec2i(714, 180));
    positions.push_back(ci::Vec2i(918, 180));
    positions.push_back(ci::Vec2i(102, 580));
    positions.push_back(ci::Vec2i(306, 580));
    positions.push_back(ci::Vec2i(510, 580));
    positions.push_back(ci::Vec2i(714, 580));
    positions.push_back(ci::Vec2i(918, 580 ));
     
     */
}
void clusterSystem::setImages(){
	stringstream s;
	for (int i=0; i< numClusters; i++) {
		s.str("");
		s << i << ".png";
		images.push_back(s.str());
	}
}
Vec2i clusterSystem::getNoiseOffset(Vec2i pos,float time)
{
	
	float nX = pos.x * 0.005f;
	float nY = pos.y * 0.005f;
	float nZ = app::getElapsedSeconds() * time;
	float noise = mPerlin.fBm( nX, nY, nZ );
	float angle = noise * 15.0f;
	Vec2f noiseVector( cos( angle ), sin( angle ) );
	return noiseVector * 40;
}

/* 
 * All the data is being taken from JSON and stored in each cluster here
 * Checks if a cluster in history mode, or live
 * takes the data, setups the forces and update Label Data
 * the Meditation and Attention is converted to Repulsion and attraction forces
 */
void clusterSystem::getAndApplyData()
{
	//Here we are going to gather all the data
	vector<int> liveClusterIDs(numClusters);
	vector<int> deadClusterIDs;
	vector<int>::iterator iter;
	string s = liveReader.mResult;
	//cout << "--------------------------------------------------------------" << endl;
	//cout << s << endl;
	
	vector<eegData> data = liveReader.parseJson(s);
	//Find out which clusters are live
	//Size of data:

	for (int i =0; i< data.size(); i++) {
		liveClusterIDs[data[i].artwork_id-1]=data[i].artwork_id;
		//cout << "LIVE ARTWORK " << data[i].artwork_id << endl;
	}
    
	//Create the vector containing the dead ones
	for (int i=0; i<liveClusterIDs.size(); i++) {
		if (liveClusterIDs[i]==0) {
			deadClusterIDs.push_back(i+1);
			
					} 

	}
	
	// setup the line colors for the Equalizer
	for (int i=0; i<numClusters; i++) {
		
		clusters[i]->theLabel.setArtwork(artwork[i]);
		clusters[i]->theLabel.equalizer.container = linecolors;
		
		if(historyReaders[i].testNet) 
			cout << "requesting " << i << endl ;
	}
	
	// WE DO NOT GET PROPER DATA HERE
	stringstream dead;
	for (int i=0; i<deadClusterIDs.size(); i++) {
				
		dead <<deadClusterIDs[i]<< " ";
		
		// if a cluster is going into the history mode, we only made the history request once for the data, declare it dead, and setup the forces to initial history modes.
		if(clusters[deadClusterIDs[i]-1]->live) {
			
			clusters[deadClusterIDs[i]-1]->particleNeighborhood = 1;
			clusters[i]->centerAttraction = 50;
			particleRepulsion = 1;
						
			historyReaders[i].mResult = "";
			historyReaders[deadClusterIDs[i]-1].startNetRequest();
			cout << " cluster " << deadClusterIDs[i] << " is dead " << endl;
			clusters[deadClusterIDs[i]-1]->live = false;
			
		}
		
		// CHeck if it is NOT live and the Dead data (history size) exists -> if so go into history mode
		if(!clusters[deadClusterIDs[i]-1]->live && clusters[deadClusterIDs[i]-1]->datadead.size() > 0)
		{
			int j = deadClusterIDs[i]-1;
			
			// history 
			
			stringstream ss;
			ss << int(clusters[j]->datadead[clusters[j]->currentJsonHistoryLine].eSenseMeditation);
			clusters[j]->theLabel.setMeditation(ss.str());
			ss.str("");
			ss << int(clusters[j]->datadead[clusters[j]->currentJsonHistoryLine].eSenseAttention);
			clusters[j]->theLabel.setAttention(ss.str());
			clusters[j]->theLabel.setArtworkID(clusters[j]->datadead[clusters[j]->currentJsonHistoryLine].artwork_id);
			
			
			
			ss.str("");
			ss << clusters[j]->datadead[clusters[j]->currentJsonHistoryLine].user_name; //"COMMUNAL MIND";
			std::string newstr = boost::to_upper_copy(ss.str()	);
			clusters[j]->theLabel.setName(newstr,artwork[j]);
			
			
			clusters[j]->repulsionTime = 2/clusters[j]->datadead[clusters[j]->currentJsonHistoryLine].eSenseMeditation+1; 
			
			clusters[j]->repulsionWidth = clusters[j]->datadead[clusters[j]->currentJsonHistoryLine].eSenseAttention;
			clusters[j]->repulsionHeight = clusters[j]->datadead[clusters[j]->currentJsonHistoryLine].eSenseMeditation;
			
			clusters[j]->particleNeighborhood = clusters[j]->datadead[clusters[j]->currentJsonHistoryLine].eSenseAttention/1.5;
			
			clusters[j]->centerAttraction = clusters[j]->datadead[clusters[j]->currentJsonHistoryLine].eSenseMeditation/4;
			particleRepulsion = 10 - clusters[deadClusterIDs[i]-1]->datadead[clusters[j]->currentJsonHistoryLine].eSenseAttention/10;
			
			
			// LOOP BACK
			if(clusters[j]->currentJsonHistoryLine >= clusters[j]->datadead.size() -1) {
				clusters[j]->currentJsonHistoryLine = 0;
			}
			else {
				clusters[j]->currentJsonHistoryLine++;
			}

		}
		
		
	}
	
	stringstream liveS;
	for (int i=0; i<liveClusterIDs.size(); i++) {
		if (liveClusterIDs[i]!=0){
		// check if the cluster is live, if it is read the json once, you do not need to do it more than once set it to history mode
			if(!clusters[liveClusterIDs[i]-1]->live) {
			
			clusters[liveClusterIDs[i]-1]->live = true;
			cout << " cluster " << liveClusterIDs[i] << " is live " << endl;
			historyReaders[liveClusterIDs[i]-1].stopNetRequest();
			
			}
		}
			liveS<< liveClusterIDs[i] << " ";
			
		
	}
	
	////////////////
	
	// check history readers if data arrived
	for(int i=0; i<	historyReaders.size(); i++) {
		
		if(historyReaders[i].fetchResultsFromServer) {
			
			if(historyReaders[i].mResult.size() > 0) {
				
				historyReaders[i].stopNetRequest();
				
				clusters[i]->datadead.clear();
				string d = historyReaders[i].mResult;
				clusters[i]->datadead = historyReaders[i].parseJson(d);
				clusters[i]->currentJsonHistoryLine = 0;
				
			
				

				
			}
		}
	}
	
	//LIVE DATA SETUP
	// setup the labels from the Data
	for (int i =0; i<data.size(); i++) {
		// Label live
		
		stringstream ss;
		ss << data[i].eSenseMeditation;
		clusters[data[i].artwork_id -1]->theLabel.setMeditation(ss.str());
		ss.str("");
		ss << data[i].eSenseAttention;
		clusters[data[i].artwork_id -1]->theLabel.setAttention(ss.str());
		
		clusters[data[i].artwork_id -1]->theLabel.setArtworkID(data[i].artwork_id);
		ss.str("");
		ss << data[i].user_name;
		clusters[data[i].artwork_id -1]->theLabel.setName(ss.str(),artwork[i]);
		
		 
		
		
		// Convert All Attention and Meditation into Attraction and Repulsion forces of the particles
		clusters[data[i].artwork_id -1]->repulsionTime =  2/data[i].eSenseMeditation+1;
		clusters[data[i].artwork_id -1]->repulsionWidth = data[i].eSenseAttention;
		clusters[data[i].artwork_id -1]->repulsionHeight = data[i].eSenseMeditation;
		clusters[data[i].artwork_id -1]->particleNeighborhood = data[i].eSenseAttention/2;
		clusters[data[i].artwork_id -1]->centerAttraction = data[i].eSenseMeditation/4;
		particleRepulsion = 10 -data[i].eSenseAttention/10;
	}
	
}


/* 
 * apply all the forces to particle systems
 */
void clusterSystem::update()
{
	if( ci::app::getElapsedSeconds() > mLastTimerFire + 0.5 ) { // every 3 seconds
		getAndApplyData();
		mLastTimerFire =  ci::app::getElapsedSeconds();
	}
	
	
	
	pSystem->setupForces();
	//Attract clusters towards their centers
	// Add repulsion time, and repulsion with
	
	for (int i=0; i<numClusters; i++) {
		
		clusters[i]->theLabel.updateContent();
		
		pSystem->addClusterAttractionForce(clusters[i]->pos.x, clusters[i]->pos.y, ci::app::App::get()->getWindowWidth(), clusters[i]->centerAttraction, i);
		Rand::randomize ();
		Vec2i offset = getNoiseOffset(Vec2i(clusters[i]->pos.x, clusters[i]->pos.y),clusters[i]->repulsionTime);
	
		pSystem->addRepulsionForce(clusters[i]->pos.x+offset.x,clusters[i]->pos.y+offset.y,clusters[i]->repulsionWidth, clusters[i]->repulsionHeight);
	}
	

	//Add per/particle cluster repulsion
	
	int n = pSystem->size();
	for (int i=0; i< n; i++){
		Particle* cur = (*pSystem)[i];
		pSystem->addClusterRepulsionForce(cur->x, cur->y, clusters[cur->clusterID]->particleNeighborhood, particleRepulsion, cur->clusterID);
		cur->live = clusters[cur->clusterID]->live;

	}
	 
	//Add mouse repulsion (affects all clusters)
	//Vec2i offset = getNoiseOffset(mousePos);
	//pSystem->addRepulsionForce(mousePos.x+offset.x,mousePos.y+offset.y,200, 90);
	
	//Apply the forces
	pSystem->applyForces();
	pSystem->update();
}
void clusterSystem::draw(){
	
	for (int i=0; i< clusters.size(); i++){
		clusters[i]->draw();
	}
	pSystem->draw();
	
}