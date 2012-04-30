#include "line.h"

Point * center;
Line * center_line;

using namespace std;

Line::Line()
{
	std::cout << "You are calling the function wrong";
	exit(0);
}
Line::Line(int _x1, int _y1, int _x2, int _y2)
{
	//	std::cout << "LINE" << std::endl;
	
	a = new Point(_x1, _y1);
	b = new Point(_x2, _y2);	

	//	std::cout << "LINE" << std::endl;
	
	// Change ID
	static int id_counter = 0;
	id = id_counter++;
	
	// Keep track of its visited history
	visited = false;
	visitedStartPoint = false;
	
	//	std::cout << "LINE" << std::endl;	
	updateCalcs();

	//	std::cout << "LINE" << std::endl;
	
	// Used for checking if we need to refresh our distance amount
	theta_cache = 3*M_PI; // some angle bigger than 2PI, aka INF	
	//distance();
	
	//	std::cout << "END LINE \n" << std::endl;	
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
	double denom = (b->x - a->x);
	if( denom == 0 )
	{
		//cout << "This program does not support perfectly verticle lines." << endl;
		//exit(0);

		// Perturb:
		b->x = b->x + 1;
		denom = (b->x - a->x);		
	}
	else
		m = (b->y - a->y)/denom;
	
	//	std::cout << m << " M " << std::endl;
	
	y_intercept = a->y - m*a->x;
	//	std::cout << y_intercept << " m " << std::endl;
}
void Line::distance()
{
	//	std::cout << "DISTANCE" << std::endl;
	
	// First find the intesection of this line and the sweep line:
	double xi = ( y_intercept - center_line->y_intercept ) / ( center_line->m - m );
	double yi = m*xi + y_intercept;

	// Now find the distance between these two lines:
	dist = sqrt( pow(center->x - xi, 2.0) + pow(center->y - yi,2.0) );

	theta_cache = center->theta;
}	
