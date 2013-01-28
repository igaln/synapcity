/*
 *  hLines.cpp
 *  synapcityLabel
 *
 *  Created by Nikolas Psaroudakis on 10/16/10.
 *  Copyright 2010 New York University. All rights reserved.
 *
 */

#include "hLines.h"
#include "cinder/Vector.h"


hLines::hLines()
{
	
	nLines = 3;
	height = 10;
	spacing = 1;
	length = 80;
	
	for (int i =0; i<nLines; i++) {
		hline hl = hline(pos+Vec2i(i*(width+spacing),6),length);
		hlines.push_back(hl);
	}
}


hLines::hLines(Vec2i _pos, int _height, int _nLines, int _spacing, int _length)
{
	nLines = _nLines;
	height = _height;
	pos = _pos;
	spacing = _spacing;
	length = _length;
	
	for (int i =0; i<nLines; i++) {
		hline l = hline(pos+ Vec2i(6,i*(height+spacing)), length);
		l.setHeight(height);
		hlines.push_back(l);
	}
	
	container.push_back(ci::Color(250,219,33));
	container.push_back(ci::Color(241,94,29));
	container.push_back(ci::Color(248,155,249));
	container.push_back(ci::Color(174,49,221));
	container.push_back(ci::Color(126,176,252));
	container.push_back(ci::Color(68, 113,187));
	container.push_back(ci::Color(103,188,53));
	container.push_back(ci::Color(147,255,26));
    
}
float hLines::getWidth()
{
	return nLines*width + (nLines-1)*spacing;
}
void hLines::setSpacing(float _spacing)
{
	
}
void hLines::draw()
{
	for (int i=0; i<container.size(); i++) {
		hlines[i].draw(container[i]);
	}
	
	
}
