#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include <iostream>

class Point
{
 public:
	int x;
	int y;
	void* parentLine;
	int id; // for removing, comparing, etc
	
	Point();
	Point(int _x1, int _y1);
	void print();
};

#endif
