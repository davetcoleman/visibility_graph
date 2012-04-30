#ifndef GEOMETRY_H_INCLUDED
#define GEOMETRY_H_INCLUDED

class Geometry
{
 public:
	//	int id; // for removing, comparing, etc

  	virtual void print() = 0;
  	virtual double value() = 0;

};

#endif
