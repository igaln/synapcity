/*
 *  hLines.h
 *  synapcityLabel
 *
 *  Edited by Igal Nassima
 *
 *	Draws equilizer of the labeling class
 */


#pragma once
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "hline.h"
#include <vector>

using namespace ci;
class hLines {
public:
	hLines();
	hLines(Vec2i _pos, int _height, int _nLines, int _spacing, int _length);
	void draw();
	void setHeight();
	void setSpacing(float _spacing);
	float getWidth();
	Vec2i pos;
	int nLines; // number of lines to user
	int spacing;	//spacing between lines
	int width;
    int height;
	int length;
	std::vector <hline> hlines;	// vector data to store the lines
	std::vector<Color> container;
};