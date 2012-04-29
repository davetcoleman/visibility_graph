#include "CImg.h"// Include CImg library header.
#include <iostream>
#include "skiplist.h"
#include <cmath>

using namespace cimg_library;
using namespace std;

//#undef min
//#undef max

const unsigned char white[] = { 255 };
const unsigned char grey[] = { 100 };
const int screen_size = 500;


//-------------------------------------------------------------------------------
//  Calculate Angle Btw 2 Vectors
//-------------------------------------------------------------------------------
double vectorsAngle( int x, int y, int basex, int basey)
{
	// Convert input point x & y to be vectors relative to base point
	int x2 = x - basex;
	int y2 = y - basey;

	// Hard code scan line to point right:
	int x1 = 1;
	int y1 = 0;

	cout << "x1: " << x1 << " - y1: " << y1 << endl;
	cout << "x2: " << x2 << " - y2: " << y2 << endl;

	double stuff = (  (x1*x2)+(y1*y2)   ) / (  sqrt(x1*x1+y1*y1)  *  sqrt(x2*x2+y2*y2) );
	cout << "Stuff: " << stuff << endl;

	// Calculate angle:	
	double result = acos( stuff );
	cout << "Result: " << result << endl;

	// Now add PI if below middle line:
	if( y > basey )
		result = 2*M_PI - result;
	
	cout << "Result: " << result*180/M_PI << " degrees" << endl;

	return result;
}
//-------------------------------------------------------------------------------
//  Main procedure
//-------------------------------------------------------------------------------
int main()
{
	cout << endl << endl << "Visibility Graph by Dave Coleman ------- " << endl << endl;

	// Variables ----------------------------------------------------------------
	
	// Line segments:
	int seg = 8;
	Line segs[] =
		{  Line(10,200,100,215),
		   Line(50,50,50,100),
		   Line(300,300,310,200), // first
		   Line(330,180,350,330), // second		   
		   Line(100,300,110,490),
		   Line(350,350,350,450),
		   Line(400,400,450,200),  // far right
		   Line(200,450,300,450)
		};

	// Temp:
	int x = screen_size / 2;
	int y = x;

	// Datastructures:
	skiplist <Point> angleList;		
	skiplist <Line> edgeList;	

	double m; // slope
	double x_intersect; // y_intersect is simply the loop iterator vertex y
	double dist; // distance from scan line point to current point
	double theta1, theta2; // holds the angls for each end point to the current center point
	Line * s;
	Point p;
		
	// Graphics:
	CImg<unsigned char> img(screen_size,screen_size,1,1,0);
	img.fill(20);

	// Algorithm -----------------------------------------------------------------
	
	// Draw segments and insert into skiplist
	for(int i = 0; i < seg; ++i)
	{
		s = &segs[i];
		
		img.draw_line(s->a.x, s->a.y, s->b.x, s->b.y, white);                    
		img.draw_circle(s->a.x, s->a.y, 2, white);
		img.draw_circle(s->b.x, s->b.y, 2, white);		

		// Calculate the angle from center line:
		//theta1 = atan2( abs( y - s->a.y ), abs( x - s->a.x ) );
		//theta2 = atan2( abs( y - s->b.y ), abs( x - s->b.x ) );
		theta1 = vectorsAngle( s->a.x, s->a.y, x, y );
		theta2 = vectorsAngle( s->b.x, s->b.y, x, y );
						
		cout << i << " - T1: " << theta1 << " - T2: " << theta2 << endl;
		cout << i << ": " << s->a.x << ", " << s->a.y << ", " << s->b.x << ", " << s->b.y << endl;
		
		// Sort the verticies:
		angleList.add(theta1, s->a);
		angleList.add(theta2, s->b);
		cout << endl;
	}

	// Test SkipList
	angleList.printAll();

	// Draw sweeper:
	img.draw_circle(screen_size / 2, screen_size / 2, 4, grey);
	img.draw_line(screen_size / 2, screen_size / 2, 450, 250, grey);

	// Initialize Edge List
	for(int i = 0; i < seg; ++i)
	{
		s = &segs[i];
		
		//s->print();
		
		// Check each line and see if it crosses scan line
		// Assume scan line is horizontal and to the right
		if( (s->a.y > y && s->b.y <= y) || (s->a.y <= y && s->b.y > y) )
		{
		    m = (s->b.y - s->a.y)/(s->b.x - s->a.x);
			x_intersect = abs( ( y - s->a.y + m * s->a.x ) / m );
			dist = abs(x - x_intersect); // because we know the line is horizontal
			//cout << "X: " << x << " Xi: " << x_intersect << " m: " << m << endl;
			cout << "Dist: " << dist << endl << endl;
			
			// It does intersect
			edgeList.add( dist, (*s) );
			img.draw_line(s->a.x, s->a.y, s->b.x, s->b.y, grey);
		}
	}

	edgeList.printAll();

	CImgDisplay disp(img, "Visibility Graph");      // Display the modified image on the screen
	
	// Sweep
	for(int i = 0; i < 4; ++i)
	{
		// take the first vertex in angular order and decide what to do with it
		p = angleList.pop();
		p.print();
		
		





		img.draw_circle(p.x, p.y, 7, grey);		
		disp.display(img);
		//usleep(250*1000);
		sleep(1);
	}
	
	// Show window until user input:
	//disp.display(img);

	while (!disp.is_closed()) {
		if (disp.is_keyESC() )
			break;
		disp.wait();
	}
	
	return EXIT_SUCCESS;
}
