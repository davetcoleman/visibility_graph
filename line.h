#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

#include <iostream>
#include "point.h"
#include "geometry.h"

class Line: public Geometry
{
 public:
	Point a;
    Point b;
	bool visited; // has the base/sweep line crossed at least one of
	              // the verticies? or was it init on it?
	int id;
	double dist; // distance from center
	
	Line();
	Line(int _x1, int _y1, int _x2, int _y2);
	virtual void print();
    virtual double value(double x);
};

#endif
