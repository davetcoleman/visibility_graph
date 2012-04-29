#include "point.h"


Point::Point()
{
	static int id_counter = 0;
	id = id_counter++;
}
Point::Point(int _x1, int _y1)
{
	x = _x1;
	y = _y1;
	Point();
}
void Point::print()
{
	std::cout << "Point x: " << x << " y: " << y << std::endl;
}

