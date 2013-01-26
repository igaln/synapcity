/*
 *  vLines.h
 *  synapcityLabel
 * 
 *  Edited by Igal Nassima
 *	
 *	Draws equilizer of the labeling class
 */


#pragma once
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "line.h"
#include <vector>

using namespace ci;
class vLines {
public:
	vLines();
	vLines(Vec2i _pos, int _width, int _nLines, int _spacing, int _length);
	void draw();
	void setHeight();
	void setSpacing(float _spacing);
	float getWidth();
	Vec2i pos;
	int nLines; // number of lines to user
	int spacing;	//spacing between lines
	int width;
	int length;
	std::vector <line> lines;	// vector data to store the lines
	std::vector<Color> container;
};