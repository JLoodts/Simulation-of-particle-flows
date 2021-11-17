//////////////////////////////////////////////////////////////////
#ifndef WALLCLASS_H												//
#define WALLCLASS_H												//
// begin of WallClass.h											//
//////////////////////////////////////////////////////////////////

#include "General.h"
#include "InputFile.h"
#include "Calculation.h"
#include <vector>
#include "Wall.h"

typedef std::vector<Wall> VECTOR;

class WallClass
{	
public:					
	WallClass();
	WallClass(InputFile inputFile);			
	VECTOR			 Vector;
	VECTOR::iterator Iterator;
	void MoveWallClass(Calculation &calculation);	
};

//////////////////////////////////////////////////////////////////
#endif															//
// end of WallClass.h											//
//////////////////////////////////////////////////////////////////