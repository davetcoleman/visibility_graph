#include "point.h"

Point::Point(){}
Point::Point(int _x1, int _y1)
{
	x = _x1;
	y = _y1;
}
void Point::print()
{
	std::cout << "Point x: " << x << " y: " << y << std::endl;
}

