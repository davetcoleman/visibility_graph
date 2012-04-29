#include "line.h"

Line::Line()
{
	static int id_counter = 0;
	visited = false;
	id = id_counter++;
}
Line::Line(int _x1, int _y1, int _x2, int _y2)
{
	a.x = _x1;
	b.x = _x2;
	a.y = _y1;
	b.y = _y2;
	Line();
}
void Line::print()
{
	std::cout << "Line: x1: " << a.x << " y1: " << a.y << " x2: " << b.x
			  << " y2: " << b.y << std::endl;
}
double Line::value(double theta)
{
	return 0; // calculate distance from midpoint at a given theta,
	          // with resepct to the baseline
}
