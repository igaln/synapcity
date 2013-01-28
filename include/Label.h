/*
 *  Label.h
 *  synapcityLabel
 *
 *  Created by Nikolas Psaroudakis on 10/16/10.
 *  Edited by Igal Nassima
 *	Creates a label, draws it on a x,y position, draws an equalizers (vlines class) and draws values for
 *	string userName;
 *	string meditValue;
 *	string attentValue;
 *	string artworkID;
 *	string artworkName;
 *
 */

#pragma once
#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include "cinder/Font.h"
#include "cinder/Rand.h"
#include "vLines.h"
#include "hLines.h"
#include "cinder/app/App.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>


using namespace ci;
using namespace gl;
using namespace std;
class	Label {
public:
	Label();
	Label(Vec2i _pos);
	void draw();
	void setData(int i, float val);
    void setMeaningData(int i, float val);
	void setArtworkID(int i);
	void setUserName(string name);
	void setAttention(string attention);
	void setMeditation(string meditation);
	void setName(string name,string artwork);
	void setArtwork(string artwork);
	void updateContent();
	
	ci::Vec2i pos;
	//string data;
	//vector <int> data;
	TextLayout nameTxt;
	TextLayout artworkTxt;
	TextLayout primaryReadingsTxt;
	
	Texture	nameTexture;
	Texture	artworkTexture;
	Texture	readingsTexture;
	
	string userName;
	string meditValue;
	string attentValue;
	string artworkID;
	string artworkName;
    
    // meaning labels
    TextLayout interestingnessTxt;
	TextLayout neurocapitalTxt;
	TextLayout artvaluationTxt;
    
    Texture interestingnessTexture;
	Texture	neurocapitalTexture;
	Texture	artvaluationTexture;
    
    string interestingness;
	string neurocapital;
	string artvaluation;
    //end meaning labels

	vLines equalizer;
    hLines meaningMeter;
	 

};