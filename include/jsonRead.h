/*
 *  jsonReader.h
 *  synapcityViz
 *
 *  Created by Nikolas Psaroudakis on 9/26/10.
 *  
 *	Header for the Json Parser class,
 *	Reads a json packet at an interval,
 *	Creates a new thread for each reader
 *	Parses the packet into vector
 *
 */

#ifndef JSON_READ_
#define JSON_READ_

#include "json.h"
#include "cinder/app/App.h"
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include "cinder/DataSource.h"
#include "cinder/Url.h"
#include "cinder/Buffer.h"
#include "cinder/Stream.h"
#include "cinder/Timer.h"
#include "cinder/thread.h"
#include "cinder/Buffer.h"


using namespace std;
using namespace boost;
using namespace detail;
/**
 Data structure to keep all the Headset data for manipulation of 
 graphics
 */
struct eegData {
	int id;
	int user_id;
	int artwork_id;
	string user_name;
	string session_created;
	
	string dateOfReading;
	float time_offset;
	
	// all the variables from the Json data from the headset
	float signalQuality;
	float eSenseAttention;
	float eSenseMeditation;
	float eegLowAlpha;
	float eegHighAlpha;
	float eegLowBeta;
	float eegHighBeta;
	float eegLowGamma;
	float eegHighGamma;
	float eegDelta;
	float eegTheta;
};
class jsonRead {
public:
	/**
	 Start a timer for calling data requests from the server
	 */
	jsonRead();
	/**
	 Start a timer for calling data requests from the server
	 */
	jsonRead(string URL);
	/**
	 * Constructor
	 */
	~jsonRead();
	/**
	 * Constructor
	 */
	void setURL(string URL);
	
	/**
	 * Read data from the given JSON URL
	 * @jsonURL : a string for the URL for json packets
	 * @returns : a string of raw json data
	 */
	string readData(string jsonURL);
	
	/**
	 * Parse json data from the given string to a vector
	 * @jsonURL : a string for the URL for json packets
	 * @returns : a vector of parsed data
	 */
	vector<eegData> parseJson(string& s);
	/**
	 * Threaded function that calls data
	 */
	void foo();
	/**
	 * Reads history json data int
	 */
	void jsonHistory();
	/**
	 * Reads regular json data
	 */
	void threadedRead();
	/**
	 * stop net request
	 */
	void stopNetRequest();
	/**
	 * start net request
	 */
	void startNetRequest();
	
	static bool local;
	
	vector<eegData> scrapedData; 
	
	Json::Value mRoot;
	Json::Reader mReader;
	
	int numClusters;
	
	string mResult;
	
	boost::shared_ptr<boost::thread> thrd;
	string mUrl;
	
	bool fetchResultsFromServer;
	bool testNet;
private:
	ci::Timer aTimer;
};
#endif