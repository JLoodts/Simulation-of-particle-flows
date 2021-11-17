#include "SearchMethod.h"

////////////////////////////////////////////////////////////////////
// BEGIN code for SearchMethod ////////////////////////////////////
SearchMethod::SearchMethod(Space &space, PartClass &part)
{
	/*	
		SearchMethod uses an array of lists from STL
	*/
	cellWidth		= 2*part.Get_RMAX();
	// number cell in the x-rich
	cellsOnARow	= (int)ceil(space.X()/cellWidth);
	// numer cells in the x-y-plane
	cellsInAPlane	= cellsOnARow*(int)ceil(space.Y()/cellWidth);
	// total number of cells in space
	NUMT nrcells = space.Z()/cellWidth;
	int nrcellen = (int)ceil(nrcells);
	numberCells	= cellsInAPlane*nrcellen;

	// initialise list by adding all particles in the appropriate cell
	array = new INTLIST[numberCells];
	//for (int i=0; i<numberCells; ++i) array[i].push_back(0);
	int cellNumber;
	for (part.Iterator = part.Vector.begin(); part.Iterator != part.Vector.end();
         part.Iterator++)
	{
		cellNumber = CellNumber(part.Iterator->Get_pos());
		array[cellNumber].push_back(part.Iterator->Get_pnr());
	}
}

void SearchMethod::UpdateList(PartClass & part)
{
	for (int thisPart = 0; thisPart<part.Vector.size(); thisPart++)
	{	
		int newCellNr = CellNumber(part.Vector[thisPart].Get_pos());
		int oldCellNr = CellNumber(part.Vector[thisPart].Get_prevPos());
		if (newCellNr != oldCellNr)
		{
			int i = *listIt;
			INTLIST::iterator tempIt;
			tempIt = listIt;
			if ((newCellNr>=0)&&(newCellNr<numberCells))
				array[newCellNr].push_back(*listIt);
			listIt++;
			i = *listIt;
			array[oldCellNr].erase(tempIt);
		}
	}
}

/*void SearchMethod::UpdateList(PartClass & part)
{
	for(int oldCellNr=0; oldCellNr<numberCells; ++oldCellNr)
		for (listIt=array[oldCellNr].begin(); 
		listIt != array[oldCellNr].end(); ++listIt) 
		{
			if ((part.Vector[*listIt]).Dead()) { 
				int i = *listIt; 
				array[oldCellNr].erase(listIt++); 
				i =*listIt;}
			else
			{
				int newCellNr = CellNumber((part.Vector[*listIt]).Get_pos());
				if (newCellNr != oldCellNr)
				{
					int i = *listIt;
					INTLIST::iterator tempIt;
					tempIt = listIt;
					if ((newCellNr>=0)&&(newCellNr<numberCells))
						array[newCellNr].push_back(*listIt);
					listIt++;
					i = *listIt;
					array[oldCellNr].erase(tempIt);
				}
			}
		}
}*/


int SearchMethod::CellNumber(Point pos)
{
	Point nrpos = pos/cellWidth;
	int cellNumber = (int)((int)floor(nrpos.z)*cellsInAPlane
						  +(int)floor(nrpos.y)*cellsOnARow
						  +(int)floor(nrpos.x)); 
	return cellNumber;
}


void SearchMethod::CheckCell(int thisPart,int nextCellNr, 
							 Calculation &calculation, PartClass &part)
/* 
	A is the particle being checked
	B is the possible collisionpartner 
*/
{				
	Particle *A,*B;
	INTLIST::iterator thisCellIt, nextCellIt;
//	for(thisCellIt=array[thisCellNr].begin(); thisCellIt != array[thisCellNr].end(); ++thisCellIt)
//	{	
		for(nextCellIt=array[nextCellNr].begin(); nextCellIt != array[nextCellNr].end(); ++nextCellIt)
		{	// perform calculations only if A.pnr < B.pnr
			//if(/**thisCellIt*/thisPart < *nextCellIt)
			{
				A = &(part.Vector[thisPart/**thisCellIt*/]);
				B = &(part.Vector[*nextCellIt]);
				if(/**thisCellIt*/thisPart != *nextCellIt)
				{
					NUMT dx = ((*A).Get_r()+(*B).Get_r())-Distance((*A).Get_pos(),(*B).Get_pos());
					if (dx>calculation.Get_DX_MAX()) calculation.Increment_nrTooDeep();	
					if (dx>=0) 
					{calculation.Part((*A),(*B),dx);}
					if (dx<0) 
					{
						dx = 0;
						calculation.dataSetIt = calculation.VectorFind((*A).Get_pnr(),Data((*B).Get_pnr(),0,0,0));
						if (calculation.dataSetIt!=calculation.VectorEnd((*A).Get_pnr())) 
							calculation.VectorErase((*A).Get_pnr(), calculation.dataSetIt);
					}
				}
			}
		}
//	}
}


void SearchMethod::CheckWalls(PartClass &part, WallClass &wall, Calculation &calculation)
{
	for (part.Iterator = part.Vector.begin(); part.Iterator != part.Vector.end(); part.Iterator++)
		for (wall.Iterator = wall.Vector.begin(); wall.Iterator != wall.Vector.end(); wall.Iterator++)
		{
			Point Q=(wall.Iterator)->ClosestPoint(part.Iterator->Get_pos());
			NUMT dx = part.Iterator->Get_r() - Distance(part.Iterator->Get_pos(),Q);

			if (dx>calculation.Get_DX_MAX()) calculation.Increment_nrTooDeep();
			if (dx>0) 
			{calculation.Walls(*(part.Iterator),Q,*(wall.Iterator),dx,calculation.Get_runTime());}

			if (dx<0) 
			{
				dx = 0;
				calculation.dataSetIt = calculation.VectorFind(part.Iterator->Get_pnr(),Data(-wall.Iterator->Get_wnr(),0,0,0));
				if (calculation.dataSetIt!=calculation.VectorEnd(part.Iterator->Get_pnr())) 
					calculation.VectorErase(part.Iterator->Get_pnr(),calculation.dataSetIt);
			}
		}
}


/*void SearchMethod::Detection(PartClass &part,WallClass &wall, 
							 Calculation &calculation, Space &space)
/* 
	if a collision is detected, Calculation is called in
*//*
{	
	for(int thisCellNr=0; thisCellNr<numberCells; thisCellNr++)
	{		
	// particle particle interaction
		int imin=0, imax=1, jmin=0, jmax=1, kmin=0, kmax=1;		
		// check neighbouring cells so that cellnr rises
		for (int k=kmin; k<=kmax; ++k)
		{ // k-> variation in the z-rich
			for (int j=jmin; j<=jmax; ++j)
			{ // j-> variation in the y-rich
				for (int i=imin; i<=imax; ++i)
				{ // i-> variation in the x-rich
					int nextCellNr = thisCellNr	+i +j*cellsOnARow +k*cellsInAPlane;
					if (nextCellNr<numberCells)
					CheckCell(thisCellNr, nextCellNr, calculation, part);				
				}
			}
		}
	}
}*/

void SearchMethod::Detection(PartClass &part,WallClass &wall, 
							 Calculation &calculation, Space &space)		
/* 
	if a collision is detected, Calculation is called in
*/
{	
	for (int thisPart = 0; thisPart<part.Vector.size(); thisPart++)
	{	
		int thisCellNr = CellNumber(part.Vector[thisPart].Get_pos());	
		int imin=-1, imax=1, jmin=-1, jmax=1, kmin=-1, kmax=1;		
		// check neighbouring cells so that cellnr rises
		for (int k=kmin; k<=kmax; ++k)
		{ // k-> variation in the z-rich
			for (int j=jmin; j<=jmax; ++j)
			{ // j-> variation in the y-rich
				for (int i=imin; i<=imax; ++i)
				{ // i-> variation in the x-rich
					int nextCellNr = thisCellNr	+i +j*cellsOnARow +k*cellsInAPlane;
					if ((nextCellNr<numberCells)&&(nextCellNr>0))
					CheckCell(thisPart, nextCellNr, calculation, part);				
				}
			}
		}
	}
}

// END code of SearchMethod ////////////////////////////////////
////////////////////////////////////////////////////////////////////