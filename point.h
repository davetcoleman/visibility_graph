#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include <iostream>
#include "geometry.h"

class Point: public Geometry
{
 public:
	int x;
	int y;
	void* parentLine;
	int id; // for removing, comparing, etc
	double theta; // anglular amount from base line
	
	Point();
	Point(int _x1, int _y1);

	virtual void print();
	virtual double value();
};

#endif
