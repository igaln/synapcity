/*
 *  Label.cpp
 *  synapcityLabel
 *
 *  Created by Nikolas Psaroudakis on 10/16/10.
 *  Edited by Igal Nassima
 * 
 */

#include "Label.h"
Label::Label()
{
	
}
Label::Label(Vec2i _pos)
{
	
	artworkTxt.setFont(Font(loadFile(cinder::app::App::get()->getResourcePath("etelkanarrowlightpro.otf")), 25 )); //etelkanarrowmediumpro.otf
	artworkTxt.setColor(Color(1.0f,1.0f,1.0f));
	nameTxt.setFont(Font( loadFile(cinder::app::App::get()->getResourcePath("etelkanarrowlightpro.otf")), 25 ));
	nameTxt.setColor(Color(1.0f,1.0f,1.0f));
	primaryReadingsTxt.setFont(Font(loadFile(cinder::app::App::get()->getResourcePath( "etelkamonospacepro.otf")), 25));
	primaryReadingsTxt.setColor(Color(1.0f,1.0f,1.0f));
	
	userName = "";
	meditValue = string("M") + string("80");
	attentValue =  string("A") + string("67");
	artworkID =  "01";
	
	artworkTxt.addLine(artworkID);
	nameTxt.addLine(userName);
	
	primaryReadingsTxt.addLine(meditValue + "|" +  attentValue);
	//primaryReadingsTxt.addLine(attentValue);
	
	readingsTexture = Texture( primaryReadingsTxt.render(true));
	nameTexture = Texture( nameTxt.render(true));
	artworkTexture = Texture(artworkTxt.render(true));
	
	pos = _pos;
	//vLines(Vec2i _pos, int _width, int _nLines, int _spacing, int _length);
	//cout << readingsTexture.getHeight() << endl;
	equalizer = vLines(Vec2i(700,300)+pos, 40, 8, 4, 200);
    meaningMeter = hLines(Vec2i(700,-400)+pos, 40, 3, 50, 250);
	
	//Set equalizer
	for (int i=0; i<equalizer.nLines; i++) {
		float r = Rand::randFloat();
		setData(i, r);
	}
    
    // meaning labels
    interestingness  = "Interestingness" ;
	neurocapital = "Neurocapital";
	artvaluation = "Art Valuation";
   
     interestingnessTxt.setFont(Font(loadFile(cinder::app::App::get()->getResourcePath("etelkanarrowlightpro.otf")), 25 ));
     interestingnessTxt.setColor(Color(1.0f,1.0f,1.0f));
     interestingnessTxt.addLine(interestingness);
    
	 neurocapitalTxt.setFont(Font(loadFile(cinder::app::App::get()->getResourcePath("etelkanarrowlightpro.otf")), 25 ));
     neurocapitalTxt.setColor(Color(1.0f,1.0f,1.0f));
     neurocapitalTxt.addLine(neurocapital);
	 artvaluationTxt.setFont(Font(loadFile(cinder::app::App::get()->getResourcePath("etelkanarrowlightpro.otf")), 25 ));
     artvaluationTxt.setColor(Color(1.0f,1.0f,1.0f));
     artvaluationTxt.addLine(artvaluation);
    
    readingsTexture = Texture( primaryReadingsTxt.render(true));

    interestingnessTexture  = Texture( interestingnessTxt.render(true));
    neurocapitalTexture  = Texture( neurocapitalTxt.render(true));
    artvaluationTexture  = Texture( artvaluationTxt.render(true));
	//Texture	neurocapitalTexture;
	//Texture	artvaluationTexture;
    
  
    //end meaning labels

}
void Label::setArtworkID(int i)
{ 
	stringstream ss;
	ss << i;
	if (i<10){
		artworkID = string("0") + ss.str();
	} else {
		artworkID = ss.str();
	}
	updateContent();

}
void Label::setName(string name, string artwork)
{

	userName = name;
	artworkName = artwork;
	
}

void Label::setArtwork(string artwork)
{
	
	artworkName = artwork;
	
}

void Label::setAttention(string attention)
{
	stringstream ss;
	ss << "A" << attention;
	
	attentValue = ss.str();
}
void Label::setMeditation(string meditation)
{
	
	//cout << "The meditation is " << meditation << endl;
	if (meditation.empty()) meditation = " ";
	stringstream ss;
	ss << "M" << meditation;
	meditValue = ss.str();
	//updateContent();
}
void Label::updateContent()
{
	//There seems to be no way to empty a TextLayout natively
	//so I moved the mLines que to public in the cinder source code.
	
	artworkTxt.mLines.clear();
	artworkTxt.addLine(artworkID + " " + artworkName);
	artworkTexture = Texture(artworkTxt.render(true));
	
	nameTxt.mLines.clear();
	nameTxt.addLine(userName);
	nameTexture = Texture(nameTxt.render(true));
	
	primaryReadingsTxt.mLines.clear();
	primaryReadingsTxt.addLine(meditValue + "|" +  attentValue);
	//primaryReadingsTxt.addLine(attentValue);
	readingsTexture = Texture( primaryReadingsTxt.render(true));
	 
}
void Label::draw()
{
	
	
	enableAlphaBlending();
	glColor3f(1.0f, 1.0f, 1.0f);
	
	//Draw artworkID, username and artworkID
	glEnable(GL_TEXTURE_2D);
    ci::gl::draw(artworkTexture, Vec2i( 700, 310 )+ pos );
	ci::gl::draw(readingsTexture,Vec2i(790,312) + pos);
	ci::gl::draw(nameTexture, pos + Vec2i(0,artworkTexture.getHeight()+equalizer.width+13));
    
    
    //new meaning labels
    ci::gl::draw(interestingnessTexture, Vec2i(705,-360)+pos);
    ci::gl::draw(neurocapitalTexture, Vec2i(705,-270)+pos);
    ci::gl::draw(artvaluationTexture, Vec2i(705,-179)+pos);
    //popMatrices();
	glDisable(GL_TEXTURE_2D);
	
	//Draw horizontal line
	//Vec2i hLinePos = Vec2i(0, artworkTexture.getHeight())+pos;
	//int hLineLength = artworkTexture.getWidth() + readingsTexture.getWidth() + equalizer.getWidth();
	
	equalizer.draw();
	meaningMeter.draw();
	
}
// val needs to be in the range 0 to 1 (i.e percentage of max line length)
void Label::setData(int i, float val)
{
	equalizer.lines[i].val = val;

}

void Label::setMeaningData(int i, float val)
{
	meaningMeter.hlines[i].val = val;
    
}