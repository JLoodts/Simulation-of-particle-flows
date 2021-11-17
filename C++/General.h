// The debugger can't handle symbols more than 255 characters long.
// STL often creates symbols longer than that.
// When symbols are longer than 255 characters, the warning is disabled.
#pragma warning(disable:4786)
//////////////////////////////////////////////////////////////////
#ifndef GENERAL_H												//
#define GENERAL_H												//
// begin of General.h											//
//////////////////////////////////////////////////////////////////

#include <math.h>

// NumberType
#define NUMT double		// for an easy conversion from doubles into floats 

// Pi
#define PI 3.141592653589793238462643

// gravity acceleration in the z-rich
#define GRAVITY -9.81
					
#include <iostream.h> // for the pseudo-errormessages

class Point
{
public:
	NUMT x,	// x-value of the point
		 y, // y-value of the point
		 z;	// z-value of the point
	Point(){}
	Point(NUMT newx, NUMT newy, NUMT newz)
			:x(newx),   y(newy),   z(newz){}

	Point operator+ (Point &B)
	{ // sum of two vectors
		Point R; R.x = x + B.x; R.y = y + B.y; R.z = z + B.z;	
		return R;}
	Point operator- (Point &B)
	{ // difference of two vectors
		Point R; R.x = x - B.x; R.y = y - B.y; R.z = z - B.z;	
		return R;}
	NUMT operator* (Point &B)
	{ // dot product between two vectors
		NUMT r; r = (x * B.x) + (y * B.y) + (z * B.z);			
		return r;}
	Point operator* (NUMT b)
	{ // vector multiplied by a scalar
		Point R; R.x = b * x; R.y = b * y; R.z = b * z;			
		return R;}
	friend Point operator* (NUMT b, Point A)
	{ // scalar multiplied by a vector
		Point R; R.x = b * A.x; R.y = b * A.y; R.z = b * A.z;	
		return R;}
	Point operator/ (NUMT b)
	{ // vector divided by a scalar
		Point R;
		if (b != 0){R.x = x / b;R.y = y / b;R.z = z / b;}
		/*else 
			::cout<<"Error: division by zero";*/
		return R;}
	void RotateOver(Point Alfa)
	{
		y = y*cos(Alfa.x)-z*sin(Alfa.x);
		z = y*sin(Alfa.x)+z*cos(Alfa.x);
		z = z*cos(Alfa.y)-x*sin(Alfa.y);
		x = z*sin(Alfa.y)+x*cos(Alfa.y);
		x = x*cos(Alfa.z)-y*sin(Alfa.z);
		y = x*sin(Alfa.z)+y*cos(Alfa.z);}
	static int Dir(Point &A)
	{// measure to detect the switching of direction
		NUMT r = A.x+A.y+A.z;
		if (r<0) return -1;
		else	 return 1;}
};															

#define ZERO Point(0,0,0)


static NUMT Distance(Point &A, Point &B)
{ // distance between two points
	NUMT x2 = (A.x-B.x)*(A.x-B.x);
	NUMT y2 = (A.y-B.y)*(A.y-B.y); 
	NUMT z2 = (A.z-B.z)*(A.z-B.z); 
	return sqrt(x2+y2+z2);
}

static Point VectorProd(Point &A, Point &B)
{ 
	Point Q; 
	Q.x=(A.y*B.z)-(A.z*B.y); 
	Q.y=(A.z*B.x)-(A.x*B.z); 
	Q.z=(A.x*B.y)-(A.y*B.x); 
	return Q;
}

static NUMT VectorLength(Point &A)
{
	return sqrt((A.x)*(A.x)+(A.y)*(A.y)+(A.z)*(A.z));
}
	
static Point NormVector(Point &A, Point &B)
{
	Point Q		= VectorProd(A,B);
	NUMT IIQII	= VectorLength(Q);
	if (IIQII==0)		return Point(0,0,0);
	else Q = Q/IIQII;	return Q;
}

//////////////////////////////////////////////////////////////////
#endif															//
// einde of General.h											//
//////////////////////////////////////////////////////////////////
