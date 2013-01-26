/*
 *  line.h
 *  synapcityLabel
 *
 *  Created by Nikolas Psaroudakis on 10/16/10.
 *  Edited by Igal Nassima
 *	
 *	Basic class to draw lines inside equilizer of the labeling class
 *
 */

#pragma once
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "cinder/gl/gl.h"
using namespace ci;
class line {
public:
	line();
	line(Vec2i _pos, int _maxHeight);
	void setValue(float _val);
	void setWidth(float _wdt);
	void setMaxHeight(int _maxHeight);
	void draw(ci::Color mcolor);
	float width;
	float maxHeight;
	float val;
	Vec2i pos;
};