#include "Main.h"

void AddParticle(Particle &A, Calculation &calculation, PartClass &part, SearchMethod &searchMethod)
{
	// deeltje A bij in de vector van deeltjes steken
	part.Increment_numParts();
	A.Clear();
	part.Vector.push_back(A);
	// de dataSet uitbreiden zodat er voor elk deeltje een lijst met botsingspartners en gegevens gemaakt kan w
	// hoeft eigenlijk niet per se aangezien bij elke updat ook geresized wordt-> gewoon zien dat je zeker niet
	// gaat rekenen voordat er geupdated is! voor de zekerheid laten staan
	calculation.VectorResize(part.Vector.size());
	// het deeltje toevoegen aan de array van cellen zodat ie mee in rekening gebracht w tijdens de volgende zoekaktie
	int cellNumber = searchMethod.CellNumber(A.Get_pos());
	searchMethod.array[cellNumber].push_back(A.Get_pnr());
}
/*
void Generate(Point &P, Point &B, Point &C, Calculation &calculation, PartClass &part, SearchMethod &searchMethod)
{
	NUMT	stepSize	= 1.1*2*part.Get_RMAX();
	NUMT	lengthB		= VectorLength(B);					NUMT	lengthC		= VectorLength(C);
	int		numStepsB	= (int)floor(lengthB/stepSize);		int		numStepsC	= (int)floor(lengthC/stepSize);
	Point	StepB		= stepSize*B/lengthB;				Point	StepC		= stepSize*C/lengthC;
	Point	FirstPos = P, SecondPos = P;
	NUMT	vel			= calculation.Get_V_MAX()/2.0;
	for (int stepB=1; stepB<numStepsB; ++stepB)
	{
		FirstPos = P + stepB*StepB;
		for (int stepC=1; stepC<numStepsC; ++stepC) 
		{
			SecondPos = FirstPos + stepC*StepC;
			//Particle(pnr,	          m,      r,      kN,  kS,  dN,  dS,  mud,  mus,  pos,	     vel,             ang, angVel);
			Particle A(part.Get_numParts(), 1.4e-4, 0.003, 7e6, 7e6, 2.06, 2.06, 0.25, 0.35, SecondPos, Point(0,0,-vel), ZERO, ZERO);
			AddParticle(A,calculation,part,searchMethod);
		}
	}
}*/

/*void Generate(Calculation &calculation, PartClass &part, SearchMethod &searchMethod)
{
	Point BottomLeftBack(0.23,0.23,0.04);
	Point Size(0.03,0.03,0.03);
	NUMT r = 0.003;
	NUMT pz = r;
	int number = part.Vector.size();
	while (pz<Size.z-r)
	{
		NUMT py = r;
		while (py<Size.y-r)
		{
			NUMT px = r;
			while (px<Size.x-r)
			{
				Point pos(px,py,pz); pos = BottomLeftBack + pos;
				Point vel(10*Random(),10*Random(),10*Random());
				Point angvel(Random(),Random(),Random());
				Particle A(number++, 1.79e-4, r, 1e+006, 1e+006, 25, 25, 0.35, 0.40, pos, vel, ZERO, ZERO);
				AddParticle(A,calculation,part,searchMethod);
				px+=2*r;
			}
			py+=2*r;
		}
		pz+=2*r;
	}
}*/

void Generate(Calculation &calculation, PartClass &part, SearchMethod &searchMethod)
{
	Point BottomLeftBack(0.1,0.1,0.1);
	Point Size(0.06,0.06,0.06);
	Point center(0.13,0.13,0.13);
	NUMT r = 0.003;
	NUMT pz = r;
	int number = part.Vector.size();
	while (pz<Size.z-r)
	{
		NUMT py = r;
		while (py<Size.y-r)
		{
			NUMT px = r;
			while (px<Size.x-r)
			{
				Point pos(px,py,pz); pos = BottomLeftBack + pos;
				if(Distance(center,pos)<0.015)
				{	
					Point vel(10*Random(),10*Random(),10*Random());
					Point angvel(Random(),Random(),Random());
					Particle A(number++, 1.79e-4, r, 1e+006, 1e+006, 25, 25, 0.35, 0.40, pos, vel, ZERO, ZERO);
					AddParticle(A,calculation,part,searchMethod);
				}
				px+=2*r;
			}
			py+=2*r;
		}
		pz+=2*r;
	}
}

bool EverythingClear(NUMT height, PartClass &part)
{
	bool clear=true;
	for(int j=0; j<part.Vector.size(); ++j)
	{
		if (part.Vector[j].Get_pos().z > (height-4*part.Get_RMAX()))
		clear=false;
	}
	return clear;
}

/**/	Timer timer;

void runRoutine()
{
	NUMT a = Random();
	/**/	TimeDelta();
	// initialisatie
	InputFile		inputFile;	
	Output			output;
	PartClass		part(inputFile);
	Calculation		calculation(inputFile,part.Get_RMIN());	
	WallClass		wall(inputFile);
	Space			space(inputFile);
	SearchMethod	searchMethod(space,part);
	/**/	timer.ini += TimeDelta();

//	Generate(calculation, part, searchMethod);

	// saving interval berekenen
	int savingInt = (int)(calculation.Get_numberLoops()/100);

	// begin van de grote for-loop
	for (int i=0; i<calculation.Get_numberLoops(); i++)

	{
//		if (EverythingClear(0.031,part))Generate(calculation, part, searchMethod);
//		Generate(Point(0,0,space.Z()-0.00301),Point(0.05,0,0),Point(0,0.05,0),calculation,part,searchMethod);
			//AddParticle(Particle(part.Get_numParts(), 1.4e-4, 0.003, 7e6, 7e6, 2.06, 2.06, 0.25, 0.35,
			//Point(0.015,0.015,space.Z()-0.00301),Point(0,0,-2),ZERO,Point(Random()/100,Random()/100,Random()/100)),calculation, part, searchMethod);

		// dynamisch deeltjes verwijderen
		//if (i==800) 
		//	part.Vector[2].RemoveThis();
		searchMethod.UpdateList(part);
		/**/	timer.update += TimeDelta();
		/**/	TimeDelta();
		searchMethod.CheckWalls(part, wall, calculation);
		/**/	timer.detectw += TimeDelta();
		searchMethod.Detection(part,wall,calculation,space);
		/**/	timer.detectp += TimeDelta();



		if (i % savingInt==0)
		output.SaveData(part,calculation,wall);
		/**/	timer.save += TimeDelta();
		part.Move(calculation);
		/**/	timer.movep += TimeDelta();
		wall.MoveWallClass(calculation);
		
		/**/	timer.movew += TimeDelta();				
		calculation.Proceed();		
		//i++; // enkel nodig bij while lus
	}
	// einde van de grote for-loop

	/**/	TimeDelta();
	output.MakeInputFile(part,space,calculation,wall);
	/**/	timer.output += TimeDelta();

	ofstream report("report.txt", ios::app);
	if (!report){cout<<"Error can't open file 'report' for output.\n";}
	// tonen vd timer-resultaten:
	report<<"\n"<<"------ Timer-results: ------"<<"\n";
	report<<setw(16)<<"Initialisatie: "	<<setw(8)<<timer.ini	<<" sec"<<"\n";
	report<<setw(16)<<"DetectWalls: "	<<setw(8)<<timer.detectw<<" sec"<<"\n";
	report<<setw(16)<<"DetectParts: "	<<setw(8)<<timer.detectp<<" sec"<<"\n";
	report<<setw(16)<<"MoveParts: "		<<setw(8)<<timer.movep	<<" sec"<<"\n";
	report<<setw(16)<<"MoveWalls: "		<<setw(8)<<timer.movew	<<" sec"<<"\n";
	report<<setw(16)<<"Update: "		<<setw(8)<<timer.update	<<" sec"<<"\n";
	report<<setw(16)<<"Save: "			<<setw(8)<<timer.save	<<" sec"<<"\n";
	report<<setw(16)<<"Output: "		<<setw(8)<<timer.output	<<" sec"<<"\n";
	report<<setw(16)<<"OverSpeed: "		<<setw(8)<<calculation.Get_nrSpeeding()<<" tms"<<"\n";
	report<<setw(16)<<"TooDeep: "		<<setw(8)<<calculation.Get_nrTooDeep()<<" tms"<<"\n";
	report<<setw(16)<<"NrParts: "		<<setw(8)<<part.Get_numParts()<<" pcs"<<"\n";
	report<<setw(16)<<"SimulationTime: "<<setw(8)<<calculation.Get_runTime()<<" sec"<<"\n";
	
}

void main()
{
srand(1);
ofstream report("report.txt", ios::out);
if (!report){cout<<"Error can't open file 'report' for output.\n";}
runRoutine();
}