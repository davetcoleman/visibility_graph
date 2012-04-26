#include "point.h"

using namespace std;

class Line
{
 public:
	point a;
	point b;
	Line(){}
	Line(int _x1, int _y1, int _x2, int _y2)
	{
		a.x = _x1;
		b.x = _x2;
		a.y = _y1;
		b.y = _y2;
	}
	void print()
	{
		cout << "Line: x1: " << a.x << " y1: " << a.y << " x2: " << b.x << " y2: " << b.y << endl;
	}
};
