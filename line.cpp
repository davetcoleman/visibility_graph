#include "line.h"

Point * center;
Line * center_line;

Line::Line()
{
	std::cout << "You are calling the function wrong";
	exit(0);
}
Line::Line(int _x1, int _y1, int _x2, int _y2)
{
	a->x = _x1;
	b->x = _x2;
	a->y = _y1;
	b->y = _y2;

	// Change ID
	static int id_counter = 0;
	visited = false;
	id = id_counter++;

	updateCalcs();

	// Used for checking if we need to refresh our distance amount
	theta_cache = -1; // temp
	distance();
	
}
void Line::print()
{
	std::cout << "Line: x1: " << a->x << " y1: " << a->y << " x2: " << b->x
			  << " y2: " << b->y << "\t ID: " << id << std::endl;
}
double Line::value()
{
	// calculate distance from midpoint at a given theta,
	// with resepct to the baseline

	if( theta_cache != center->theta ) // check if our cached version is still fresh enough
	{
		distance();
	}

	return dist;
}
void Line::updateCalcs()
{
	// Find Slope and y-intercept of this line for future distance calculations
	m = (b->y - a->y)/(b->x - a->x);
	y_intercept = a->y - m*a->x;
}
void Line::distance()
{
	// First find the intesection of this line and the sweep line:
	double xi = ( y_intercept - center_line->y_intercept ) / ( center_line->m - m );
	double yi = m*xi + y_intercept;

	// Now find the distance between these two lines:
	dist = sqrt( pow(center->x - xi, 2.0) + pow(center->y - yi,2.0) );

	theta_cache = center->theta;
}	
