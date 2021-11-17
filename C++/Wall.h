//////////////////////////////////////////////////////////////////
#ifndef WALL_H													//
#define WALL_H													//
// begin of Wall.h												//
//////////////////////////////////////////////////////////////////

#include "General.h"

class Wall
{
public:
	Wall(){}
	Wall(int newwnr, Point newA, Point newB, Point newP, Point newDeltaP, Point newR, Point newamp, Point newfreq, Point newangVel)
		:wnr (newwnr), a(VectorLength(newA)), A(newA/VectorLength(newA)),
		b(VectorLength(newB)), B(newB), P(newP), DeltaP(newDeltaP), R(newR),
		amp(newamp), rfreq(2*PI*newfreq), angVel(newangVel)
	{
		if ((A * B !=0)&&(wnr != 0)) 
		{
			::cout<<"Error: A and B should be perpendicular";
			Point C=VectorProd(A,B); B=NormVector(C,A);
		}
		if (wnr!=0) B = B/VectorLength(B);
	}
	int		Get_wnr(){return wnr;}
	Point	Get_R()	 {return R;}	
	Point	ClosestPoint(Point &C);
	Point	VelTrans(NUMT runTime);
	Point	VelRot(Point &C);
		void MakeInput(const char *fileName);
static	void MoveWall(Wall &wall, NUMT DT, NUMT runTime);
private:
	// general
	int		wnr;
	Point	A,B,	// 2 normalised vect putting up the plane
			P,		// points to the origin of the plane from R
			DeltaP,	// to allow small vibrations
			R;		// points to the rotating point
	NUMT	a,b;	// the length of the plane in the A- and B-dir
	// kinetics
	Point	amp,	// amplitude of the sine for DeltaP
			rfreq,	// omega of the sine for DeltaP
			angVel;	// rotating velocity of the wall		
};

//////////////////////////////////////////////////////////////////
#endif															//
// end of Wall.h												//
//////////////////////////////////////////////////////////////////