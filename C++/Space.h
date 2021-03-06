//////////////////////////////////////////////////////////////////
#ifndef SPACE_H													//
#define SPACE_H													//
// begin of Space.h												//
//////////////////////////////////////////////////////////////////

#include "InputFile.h"
#include "Wall.h"

class Space
{	
public:	
	Space(InputFile inputFile);
	NUMT X() {return constX;}
	NUMT Y() {return constY;}
	NUMT Z() {return constZ;}
private:
	NUMT constX,
		 constY,
		 constZ;	
};

//////////////////////////////////////////////////////////////////
#endif															//
// end of Space.h												//
//////////////////////////////////////////////////////////////////