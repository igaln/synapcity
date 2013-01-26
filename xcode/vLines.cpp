/*
 *  vLines.cpp
 *  synapcityLabel
 *
 *  Created by Nikolas Psaroudakis on 10/16/10.
 *  Copyright 2010 New York University. All rights reserved.
 *
 */

#include "vLines.h"
#include "cinder/Vector.h"


vLines::vLines()
{
	
	nLines = 8;
	width = 3;
	spacing = 1;
	length =30;
	
	
	
	
	
	for (int i =0; i<nLines; i++) {
		line l = line(pos+Vec2i(i*(width+spacing),6),length);
		lines.push_back(l);
	}
}
vLines::vLines(Vec2i _pos, int _width, int _nLines, int _spacing, int _length)
{
	nLines = _nLines;
	width = _width;
	pos = _pos;
	spacing = _spacing;
	length = _length;
	
	for (int i =0; i<nLines; i++) {
		line l = line(pos+Vec2i(i*(width+spacing),6), length);
		l.setWidth(width);
		lines.push_back(l);
	}
	/*
	container.push_back(ci::Color(250,219,33));
	container.push_back(ci::Color(241,94,29));
	container.push_back(ci::Color(248,155,249));
	container.push_back(ci::Color(174,49,221));
	container.push_back(ci::Color(126,176,252));
	container.push_back(ci::Color(68, 113,187));
	container.push_back(ci::Color(103,188,53));
	container.push_back(ci::Color(147,255,26));
	 */
}
float vLines::getWidth()
{
	return nLines*width + (nLines-1)*spacing;
}
void vLines::setSpacing(float _spacing)
{
	
}
void vLines::draw()
{
	for (int i=0; i<container.size(); i++) {
		lines[i].draw(container[i]);
	}
	
	
}
