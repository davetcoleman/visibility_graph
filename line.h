#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

#include <iostream>
#include "point.h"
#include "geometry.h"
#include <cmath>

class Line: public Geometry
{
 public:
	Point * a;
    Point * b;
	bool visitedStartPoint; // has the base/sweep line crossed at least one of
	              // the verticies? 
	bool visited;  // has the sweep line been on the line (as in, maybe it was init on it)
	
	int id;
	double dist; // distance from center
	double theta_cache; // used for deciding if the dist cache needs to be refreshed
	double m; // slope of line
	double y_intercept; // y-intercept of line
	
	Line();
	Line(int _x1, int _y1, int _x2, int _y2);
	virtual void print();
    virtual double value();

	void updateCalcs();
	void distance();
	void center_intercept(double &xi, double &yi);
};


// This global needs to be visible to classes:
extern Point * center;
extern Line * center_line;


#endif
