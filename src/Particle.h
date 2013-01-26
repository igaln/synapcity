/*
 *  clusterSystem.h
 *  synapcityVizV3
 *
 *  Created by Igal Nassima on 10/31/10.
 *
 *	Simple particle class that takes a vector and color to draw itself,
 *  Additionally if system is in History mode, it can change colors
 *
 */


#pragma once
#include <vector>
#include <cmath>
#include "cinder/gl/gl.h"
#include "cinder/Color.h"
using namespace std;
using namespace ci;

class Particle {
public:
	float x, y;
	float xv, yv;
	float xf, yf;
	ColorA	color;
	int clusterID;
	bool live;
	
	Particle(float _x = 0, float _y = 0,float _xv = 0, float _yv = 0, ci::ColorA _color = ci::ColorA() , int _clusterID = 0) :
	x(_x), y(_y),xv(_xv), yv(_yv), color(_color), clusterID(_clusterID) 
	{
		
		
	}
	void updatePosition(float timeStep) {
		// f = ma, m = 1, f = a, v = int(a)
		xv += xf;
		yv += yf;
		x += xv * timeStep;
		y += yv * timeStep;
	}
	void resetForce() {
		xf = 0;
		yf = 0;
	}
	void bounceOffWalls(float left, float top, float right, float bottom, float damping = .3) {
		bool collision = false;

		if (x > right){
			x = right;
			xv *= -1;
			collision = true;
		} else if (x < left){
			x = left;
			xv *= -1;
			collision = true;
		}

		if (y > bottom){
			y = bottom;
			yv *= -1;
			collision = true;
		} else if (y < top){
			y = top;
			yv *= -1;
			collision = true;
		}

		if (collision == true){
			xv *= damping;
			yv *= damping;
		}
	}
	void addDampingForce(float damping = .01) {
		xf = xf - xv * damping;
    yf = yf - yv * damping;
	}
	void draw() {
		//glColor4f(color);
		
		if(live) {
			glColor4f(color);
		}
		else {
			glColor4f(1.0f,1.0f,1.0f,0.6f);
		}
		

		glVertex2f(x, y);
	}
};
