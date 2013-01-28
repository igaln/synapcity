/*
 *  hline.cpp
 *  synapcityLabel
 *
 *  Created by Igal Nassima 
 *  Copyright 2010 New York University. All rights reserved.
 *
 */

#include "hline.h"
#include <iostream>

hline::hline()
{
	maxWidth = 94;
	width = 10;
	pos = Vec2i(0,0);
	val = 1.0f;
}
hline::hline(Vec2i _pos, int _maxWidth)
{
	pos=_pos;
	maxWidth = _maxWidth;
	width = 10;
	val =1;
}
void hline::setValue(float _val)
{
	val = _val;
}
void hline::setHeight(float _hgt)
{
	height = _hgt;
}
void hline::setMaxWidth(int _maxWidth)
{
	maxWidth = _maxWidth;
}
void hline::draw(ci::Color mcolor)
{
    //glColor3i(250,219,33);
	glColor3ub(mcolor.r,mcolor.g,mcolor.b);
	////250,219,33);
	glBegin(GL_QUADS);
	glVertex2f(pos.x, pos.y);
	glVertex2f(pos.x + val*maxWidth, pos.y);
	glVertex2f(pos.x + val*maxWidth, pos.y+height);
	glVertex2f(pos.x, pos.y+height);
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
}