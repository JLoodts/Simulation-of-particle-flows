//////////////////////////////////////////////////////////////////
#ifndef OUTPUT_H												//
#define OUTPUT_H												//
// begin of Output.h											//
//////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <iostream.h>
#include <iomanip.h>
#include <fstream.h>
#include "PartClass.h"
#include "Space.h"
#include "Calculation.h"
#include "WallClass.h"
#include "SearchMethod.h"

class Output
{
public:
	Output();
	void	 MakeInputFile(PartClass &part, Space &space, Calculation &calculation,
					   WallClass &wall);
	void	 SaveData(PartClass &part, Calculation &calculation, WallClass &wall);
private:
	char	 buf[99];
	int		 n;
	ofstream gg;
};

//////////////////////////////////////////////////////////////////
#endif															//
// end of Output.h												//
//////////////////////////////////////////////////////////////////