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
	
	artworkTxt.setFont(Font(loadFile(cinder::app::App::get()->getResourcePath("etelkanarrowlightpro.otf")), 18 )); //etelkanarrowmediumpro.otf
	artworkTxt.setColor(Color(1.0f,1.0f,1.0f));
	nameTxt.setFont(Font( loadFile(cinder::app::App::get()->getResourcePath("etelkanarrowlightpro.otf")), 15 ));
	nameTxt.setColor(Color(1.0f,1.0f,1.0f));
	primaryReadingsTxt.setFont(Font(loadFile(cinder::app::App::get()->getResourcePath( "etelkamonospacepro.otf")), 15));
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
	equalizer = vLines(Vec2i(2,8)+pos, 4, 8, 2, 14-4);
	
	//Set equalizer
	for (int i=0; i<equalizer.nLines; i++) {
		float r = Rand::randFloat();
		setData(i, r);
	}

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
		ci::gl::draw(artworkTexture, Vec2i( 0, 16 )+ pos );
	ci::gl::draw(readingsTexture,Vec2i(51,2) + pos);
	ci::gl::draw(nameTexture, pos + Vec2i(0,artworkTexture.getHeight()+equalizer.width+13));
    //popMatrices();
	glDisable(GL_TEXTURE_2D);
	
	//Draw horizontal line
	Vec2i hLinePos = Vec2i(0, artworkTexture.getHeight())+pos;
	int hLineLength = artworkTexture.getWidth() + readingsTexture.getWidth() + equalizer.getWidth();
	
	equalizer.draw();
		 
	
}
// val needs to be in the range 0 to 1 (i.e percentage of max line length)
void Label::setData(int i, float val)
{
	equalizer.lines[i].val = val;

}