/*
 *  jsonReader.cpp
 *  synapcityViz
 *
 *  Created by Nikolas Psaroudakis on 9/26/10.
 *	Edited by Igal Nassima Since then.
 *
 */

#include "jsonRead.h"

bool jsonRead::local = false;

jsonRead::jsonRead():fetchResultsFromServer(false),testNet(false)
{
		aTimer = ci::Timer();

	
}
jsonRead::jsonRead(string URL):fetchResultsFromServer(true), mUrl(URL)
{
	aTimer = ci::Timer();	
}
void jsonRead::setURL(string URL)
{
	mUrl = URL;
}
jsonRead::~jsonRead()
{
	
}
void jsonRead::foo()
{
	while (true) {
		usleep(500000);
		testNet = false;
		if (fetchResultsFromServer){
			mResult = readData(mUrl); 
			testNet = true;
		}
	}	
}

void jsonRead::stopNetRequest() {
	
	fetchResultsFromServer = false;
}
void jsonRead::startNetRequest() {
	
	fetchResultsFromServer = true;
}


void jsonRead::threadedRead()
{
	
	assert(!thrd);
	thrd = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&jsonRead::foo, this)));
}

string jsonRead::readData(string jsonURL)
{

		string tResult;
		if (local) {
			//string path = cinder::app::App::get()->getResourcePath("readingOrig.json");
			string path = cinder::app::App::get()->getResourcePath("mathias_wave.json");
			ifstream infile(path.c_str());
			string mRes((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
			mResult = mRes;
		} else {
			//mResult = "NOT LOCAL";
			string theURL= jsonURL;
			string user = "synapcity";
			string pass = "acclair";
			cinder::IStreamUrlRef urlRef = cinder::loadUrlStream(theURL,user,pass); 
			tResult="";
			
			if(urlRef)
			{ 
				
				// use buffer
				ci::Buffer buf = loadStreamBuffer( urlRef );
				unsigned char* j = (unsigned char*) buf.getData();
				
				
				tResult.assign( j,j+buf.getDataSize());
				buf.reset();
				buf = NULL;
			
			
			}
		 }
		//Fix JSON DOM
		tResult = "{\"output\":" + tResult;
		tResult+="}";
		
		//cout << "--------------- resut from thread " <<  tResult << endl;
	

	return tResult;	
}
vector<eegData> jsonRead::parseJson(string& s)
{
	scrapedData.clear();
	string result;
	bool wasSuccessfull = mReader.parse(s,mRoot);
	//cout << "The size of mRoot is: " << mRoot.size() << endl;
	if (!wasSuccessfull || s.size() < 50 || mRoot.size() > 40)
	{
		//result="JSON PARSING FAILED";
		cout << result << endl;
		return scrapedData;		
	} else {
		
		cout << "JSON PARSING SUCCESS" << endl;
	}
	Json::Value rootOut = mRoot["output"];
	//cout << rootOut.size() << endl;
	
	for (int i=0; i<rootOut.size(); ++i) {
		Json::Value elemOut = rootOut[i];
		//string val = elemOut.get()
		//elemOut = elemOut["reading"]["id"];
		eegData dataIn;
		
		Json::Value reading = elemOut["reading"];
		
		
		
		dataIn.id = reading.get("id",0).asInt();
		dataIn.user_id = reading.get("user_id",0).asInt();
		dataIn.user_name = reading.get("user_name",0).asString();
		dataIn.artwork_id = reading.get("artwork_id",0).asInt();
		dataIn.session_created = reading.get("session_created"," ").asString();
		dataIn.dateOfReading = reading.get("dateOfReading"," ").asString();
		dataIn.time_offset = reading.get("time_offset",0.0).asDouble();
		//dataIn.signalQuality = elemOut["reading"]["signalQuality"].asDouble();
		dataIn.eSenseAttention = reading.get("eSenseAttention", 0.0).asDouble();
		dataIn.eSenseMeditation  = reading.get("eSenseMeditation", 0.0).asDouble();
		//cout << "THe fucking medit is: " << dataIn.eSenseMeditation << endl;
		
		
		/*
		dataIn.id = elemOut["reading"]["id"].asInt();
		dataIn.user_id = elemOut["reading"]["user_id"].asInt();
		dataIn.artwork_id = elemOut["reading"]["artwork_id"].asInt();
		dataIn.session_created = elemOut["reading"]["session_created"].asString();
		dataIn.dateOfReading = elemOut["reading"]["dateOfReading"].asString();
		dataIn.time_offset = elemOut["reading"]["time_offset"].asDouble();
		//dataIn.signalQuality = elemOut["reading"]["signalQuality"].asDouble();
		dataIn.eSenseAttention = elemOut["reading"]["eSenseAttention"].asDouble();
		dataIn.eSenseMeditation  = elemOut["reading"]["eSenseMeditation"].asDouble();
		
		*/
		
		//dataIn.eegLowAlpha  = elemOut["reading"]["eegLowAlpha"].asDouble();
		//dataIn.eegHighAlpha  = elemOut["reading"]["eegHighAlpha"].asDouble();
		//dataIn.eegLowBeta  = elemOut["reading"]["eegLowBeta"].asDouble();
		//dataIn.eegHighBeta  = elemOut["reading"]["eegHighBeta"].asDouble();
		//dataIn.eegLowGamma  = elemOut["reading"]["eegLowGamma"].asDouble();
		//dataIn.eegHighGamma  = elemOut["reading"]["eegHighGamma"].asDouble();
		//dataIn.eegDelta  = elemOut["reading"]["eegDelta"].asDouble();
		//dataIn.eegTheta  = elemOut["reading"]["eegTheta"].asDouble();
		scrapedData.push_back(dataIn);
	}
	return scrapedData;
}

