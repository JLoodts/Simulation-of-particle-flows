//////////////////////////////////////////////////////////////////
#ifndef CALCULATION_H											//
#define CALCULATION_H											//
// begin van Calculation.h										//
//////////////////////////////////////////////////////////////////

#include "InputFile.h"
#include "General.h"
#include "Particle.h"
#include "Wall.h"
#include <vector>		// for vector
#include <math.h>		// for sqrt 
#include <set>			// for dataset


// TEMPLATE STRUCT smallerthen
template<class T>
struct smallerthen  : std::binary_function<T, T, bool> 
	{
		bool operator()(const T& X, const T& Y) const
			{return (X.partner < Y.partner); }
	};


class Data
/*
	this will be the data elements in the contact lists
*/
{
public:
	Data(){}
	Data(int newpartner, bool newslide, NUMT newdxTSum, int newdirection)
			:partner(newpartner), slide(newslide), dxTSum(newdxTSum), direction(newdirection){}
	int  partner;
	/* 
		partner == -6 -> wallnr 6 
		partner == 0  -> wallnr 0
		partner == 6  -> partnr 6
	*/
	bool slide; 
	NUMT dxTSum;	// tang distance traveled since beginning of deformation
	int	 direction; // dir of velocity at the beginning of deformation
};

//set<TYPE, PREDICATE, ALLOCATOR>;
typedef std::set<Data, smallerthen<Data>, std::allocator<Data> > DATASET;
typedef std::vector<DATASET> DATASETVECTOR;

class Calculation
{
public:
	Calculation(InputFile inputFile, NUMT rmin);

	NUMT	Get_DT() { return DT; }
	NUMT	Get_numberLoops() { return numberLoops; }
	NUMT	Get_nrSpeeding() { return nrSpeeding; }
	NUMT	Get_nrTooDeep() { return nrTooDeep; }
	NUMT	Get_runTime() { return runTime; }
	NUMT	Get_DX_MAX() { return DX_MAX; }
	NUMT	Get_V_MAX() { return V_MAX; }
	void	Increment_nrSpeeding(){nrSpeeding++;}
	void	Increment_nrTooDeep(){nrTooDeep++;}
	void	Proceed(){runTime += DT;}
	void	VectorResize(int numberParticles){DATASET dataSet; Vector.resize(numberParticles,dataSet);}	
	void	VectorErase(int pnr, DATASET::iterator dataSetIt){Vector[pnr].erase(dataSetIt);}
	void	Part(Particle &A,Particle &B,NUMT dx);
	void	Walls(Particle &A,Point &S,Wall &wall,NUMT dx,NUMT runTime);
	DATASET::iterator VectorFind(int pnr, Data data){return Vector[pnr].find(data);}
	DATASET::iterator VectorEnd(int pnr){return Vector[pnr].end();}
	DATASET::iterator dataSetIt; 
private:  
	DATASETVECTOR	Vector;		// vector[A] is the contactlist of particle A 
	DATASETVECTOR::iterator Iterator;
	NUMT	DT;
	int		numberLoops;
	NUMT	runTime;
	NUMT	DX_MAX;		// = 1% rmin
	int		nrTooDeep;  // DX exceeds DX_MAX
	NUMT	V_MAX;		// = DX_MAX/DT
	int		nrSpeeding; // number times of overspeeding
};

//////////////////////////////////////////////////////////////////
#endif															//
// end of Calculation.h											//
//////////////////////////////////////////////////////////////////
