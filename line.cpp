#include "line.h"

Point * center;
Line * center_line;
double atomic;

using namespace std;

Line::Line()
{
	cout << "You are calling the function wrong";
	exit(0);
}
Line::Line(int x1, int y1, int x2, int y2)
{
	// Order a and b such that a.x > b.x
	if( x1 > x2 )
	{
		a = new Point(x1, y1);
		b = new Point(x2, y2);
	}
	else
	{
		b = new Point(x1, y1);
		a = new Point(x2, y2);
	}
	
	// Change ID
	static int id_counter = 0;
	id = id_counter++;
	
	// Keep track of its visited history
	visited = false;
	visitedStartPoint = false;
	
	//	cout << "LINE" << endl;	
	updateCalcs();

	//	cout << "LINE" << endl;
	
	// Used for checking if we need to refresh our distance amount
	theta_cache = 3*M_PI; // some angle bigger than 2PI, aka INF	
	//distance();
	
	//	cout << "END LINE \n" << endl;	
}
Line::~Line()
{
	//delete a;
	//delete b;
}
void Line::print()
{
	cout << "Line: x1: " << a->x << " y1: " << a->y << " x2: " << b->x
			  << " y2: " << b->y << "\t ID: " << id << endl;
}
double Line::value()
{
	// calculate distance from midpoint at a given theta,
	// with resepct to the baseline

	if( theta_cache != center->theta ) // check if our cached version is still fresh enough
	{
		//cout << "Recalculaing distance for line " << id << endl;
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
		//		b->x = b->x + 1;
		denom = 0.000000001; //(b->x - a->x);		
	}
	m = (b->y - a->y)/denom;
	
	//	cout << m << " M " << endl;
	
	y_intercept = a->y - m*a->x;
	//	cout << y_intercept << " m " << endl;
}
void Line::distance()
{
	// First find the intesection of this line and the sweep line:
	double xi;
	double yi;
	center_intercept( xi, yi );

	//cout << "The intercept is x: " << xi << " y: " << yi << endl;
	//cout << "M: " << m << " b: " << y_intercept << endl;
	
	// Now find the distance between these two lines:
	dist = sqrt( pow(center->x - xi, 2.0) + pow(center->y - yi,2.0) );

	//cout << "Distance: " << dist << endl << endl;
	theta_cache = center->theta;
}	

void Line::center_intercept(double &xi, double &yi)
{
	xi = double( y_intercept - center_line->y_intercept ) / double( center_line->m - m );
	yi = m*xi + y_intercept;
}
