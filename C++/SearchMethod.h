//////////////////////////////////////////////////////////////////
#ifndef SEARCHMETHOD_H											//
#define SEARCHMETHOD_H											//
// begin van SearchMethod.h										//
//////////////////////////////////////////////////////////////////

// turn off warning about symbols too long for debugger
#pragma warning (disable : 4786)

#include <list>			// for list
#include <stdlib.h>		// for list

#include <math.h>
#include "General.h"
#include "InputFile.h"
#include "PartClass.h"
#include "WallClass.h"
#include "Calculation.h"
#include "Space.h"

typedef std::list<int> INTLIST;

class SearchMethod
{	
public:
	SearchMethod(Space &space, PartClass &part);
// ERROR // destructor als deze niet als inline w geimplementeerd is er een conflict met de destructor 
// ERROR // die vanuit STL w aangemaakt
	~SearchMethod(){delete[] array;};	
	INTLIST *array; 
	int  CellNumber(Point pos);
	void UpdateList(PartClass & part);
	void Detection(PartClass &part, WallClass &wall, Calculation &calculation, Space &space);
	void CheckWalls(PartClass &part, WallClass &wall, Calculation &calculation);
private:   
	INTLIST::iterator listIt;
	NUMT cellWidth;
	int  cellsOnARow;
	int  cellsInAPlane;
	int  numberCells;
	void CheckCell(int thisPart, int nextCellNr, Calculation &calculation, PartClass &part);	
};

//////////////////////////////////////////////////////////////////
#endif															//
// end of SearchMethod.h										//
//////////////////////////////////////////////////////////////////