//#ifndef POINT_H_INCLUDED
//#define POINT_H_INCLUDED

//#include "line.h"

class Point
{
 public:
	int x;
	int y;
	//	Line *parent;
	
	Point(){}
	Point(int _x1, int _y1);
	void print();
};

//#endif
