/*
 *  line.h
 *  synapcityLabel
 *
 *  Created by Igal Nassima
 *
 *	Basic class to draw lines inside equilizer of the labeling class
 *
 */

#pragma once
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "cinder/gl/gl.h"
using namespace ci;
class hline {
public:
	hline();
	hline(Vec2i _pos, int _maxHeight);
	void setValue(float _val);
	void setHeight(float _hgt);
	void setMaxWidth(int _maxHeight);
	void draw(ci::Color mcolor);
	float width;
    float height;
	float maxWidth;
	float val;
	Vec2i pos;
};