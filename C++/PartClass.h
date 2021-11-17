//////////////////////////////////////////////////////////////////
#ifndef PARTCLASS_H												//
#define PARTCLASS_H												//
// begin of PartClass.h										//
//////////////////////////////////////////////////////////////////

#include "General.h"
#include "InputFile.h"
#include "Calculation.h"
#include <vector>
#include "Particle.h"

typedef std::vector<Particle> PARTVECTOR;

class PartClass
{	
public:					
	PartClass();
	PartClass(InputFile inputFile);
	int		Get_numParts()		{return numParts;}
	NUMT	Get_RMAX()			{return RMAX;} 	
	NUMT	Get_RMIN()			{return RMIN;}
	void	Increment_numParts(){numParts++;}
	void	Clear();
	void	Move(Calculation & calculation);
	PARTVECTOR			 Vector;	
	PARTVECTOR::iterator Iterator;
private:
	int		numParts;
	NUMT	RMAX;			// max radius of the particles
	NUMT	RMIN;			// min radius of the particles
};

//////////////////////////////////////////////////////////////////
#endif															//
// einde of PartClass.h											//
//////////////////////////////////////////////////////////////////