#include "WallClass.h"

////////////////////////////////////////////////////////////////////
// BEGIN code for wallClass ////////////////////////////////////////
WallClass::WallClass(InputFile iF)
{
	iF.myFind("-walls-");
	iF.get("numberwalls");
	int imax = atoi((*iF.location).c_str());// number walls
	Point A,B,P,DeltaP,R,amp,freq,angVel;
	int wnr;
	for (int i=0; i<imax; ++i)
	{
		iF.get("wnr");		wnr		= atoi((*iF.location).c_str());
		iF.get("ax");		A.x		= atof((*iF.location).c_str()); 
		iF.get("ay");		A.y		= atof((*iF.location).c_str());
		iF.get("az");		A.z		= atof((*iF.location).c_str());
		iF.get("bx");		B.x		= atof((*iF.location).c_str());
		iF.get("by");		B.y		= atof((*iF.location).c_str());
		iF.get("bz");		B.z		= atof((*iF.location).c_str());
		iF.get("px");		P.x		= atof((*iF.location).c_str());
		iF.get("py");		P.y		= atof((*iF.location).c_str());
		iF.get("pz");		P.z		= atof((*iF.location).c_str());
		iF.get("dpx");		DeltaP.x= atof((*iF.location).c_str());
		iF.get("dpy");		DeltaP.y= atof((*iF.location).c_str());
		iF.get("dpz");		DeltaP.z= atof((*iF.location).c_str());
		iF.get("rx");		R.x		= atof((*iF.location).c_str());
		iF.get("ry");		R.y		= atof((*iF.location).c_str());
		iF.get("rz");		R.z		= atof((*iF.location).c_str());
		iF.get("ampx");		amp.x	= atof((*iF.location).c_str());
		iF.get("ampy");		amp.y	= atof((*iF.location).c_str());
		iF.get("ampz");		amp.z	= atof((*iF.location).c_str());
		iF.get("freqx");	freq.x	= atof((*iF.location).c_str());
		iF.get("freqy");	freq.y	= atof((*iF.location).c_str());
		iF.get("freqz");	freq.z	= atof((*iF.location).c_str());
		iF.get("angvelx");	angVel.x= atof((*iF.location).c_str());
		iF.get("angvely");	angVel.y= atof((*iF.location).c_str());
		iF.get("angvelz");	angVel.z= atof((*iF.location).c_str());
		Wall wall(wnr,A,B,P,DeltaP,R,amp,freq,angVel);
		Vector.push_back(wall);
	}
}


Point Wall::ClosestPoint(Point &C)
{
	if (wnr==0)
	{ // conical disc
		Point newC = C-P-DeltaP-R;
		NUMT distanceToAngVel = sqrt(newC.x*newC.x+newC.y*newC.y);
		if (distanceToAngVel<B.x)
		{// particle is located above central flat part
			Point Q(newC.x,newC.y,B.z*B.x);
			return Q + R + P + DeltaP;
		}else{
		// check wether the part is located above the disc
		if (distanceToAngVel >= B.y) 
		{// suppose closest point is center of disc -> certainly no contact	
			return R + P + DeltaP;
		}else{
		// particle hits the disc
			Point Q(newC.x, newC.y, distanceToAngVel*B.z);
			return Q + R + P + DeltaP;
		}// end of else from >= r
		}// end of else from !VectorProd
	}else{
		Point newC = C-P-DeltaP-R;
		// aco, bco are the local coordinates of newC
		NUMT aco = newC * A;
		if (aco<0) aco = 0;
		else	if (aco>a) aco = a;
		NUMT bco = newC * B;
		if (bco<0) bco = 0;
		else 	if (bco>b) bco = b;
		Point Q = R + P + DeltaP + (aco * A + bco * B);
		return Q;
	}
}

Point Wall::VelTrans(NUMT runTime)
{
	Point R = runTime*rfreq;
	R.x = cos(R.x); R.y = cos(R.y); R.z = cos(R.z);
	R = amp*rfreq*R;
	return R;
}

Point Wall::VelRot(Point &C)
{
	return VectorProd(angVel,C);
}

void Wall::MakeInput(const char *fileName)
{
	std::ofstream outfile(fileName, ios::app);
	outfile	<<"wnr "	<<wnr
			<<" ax "	<<A.x * a
			<<" ay "	<<A.y * a
			<<" az "	<<A.z * a
			<<" bx "	<<B.x * b
			<<" by "	<<B.y * b
			<<" bz "	<<B.z * b
			<<" px "	<<P.x
			<<" py "	<<P.y
			<<" pz "	<<P.z
			<<" dpx "	<<DeltaP.x
			<<" dpy "	<<DeltaP.y
			<<" dpz "	<<DeltaP.z
			<<" rx "	<<R.x
			<<" ry "	<<R.y
			<<" rz "	<<R.z
			<<" ampx "	<<amp.x
			<<" ampy "	<<amp.y
			<<" ampz "	<<amp.z
			<<" freqx "	<<rfreq.x/(2*PI)
			<<" freqy "	<<rfreq.y/(2*PI)
			<<" freqz "	<<rfreq.z/(2*PI)
			<<" angvelx "	<<angVel.x
			<<" angvely "	<<angVel.y
			<<" angvelz "	<<angVel.z
			<<"\n";
}

void WallClass::MoveWallClass(Calculation &calculation)		
{	
	NUMT DT = calculation.Get_DT();
	NUMT runTime = calculation.Get_runTime();
	for (Iterator = Vector.begin()+1; Iterator != Vector.end();
         Iterator++)
	{// first wall is the conical disc, which should not be rotated
		 Wall::MoveWall(*Iterator,DT,runTime);
	}
}

void Wall::MoveWall(Wall &wall, NUMT DT, NUMT runTime)		
{	
	/*
		translation trough deltaP 
		this is a vibrating sine with rfreq and amp
		rotating with angVel
	*/
	Point DeltaP(wall.rfreq*runTime);
	DeltaP.x = sin(DeltaP.x); DeltaP.y = sin(DeltaP.y); DeltaP.z = sin(DeltaP.z);
	DeltaP.x = wall.amp.x*DeltaP.x; DeltaP.y = wall.amp.y*DeltaP.y; DeltaP.z = wall.amp.z*DeltaP.z;
 	wall.DeltaP = DeltaP;
	wall.A.RotateOver(DT*wall.angVel);
	wall.B.RotateOver(DT*wall.angVel);
	wall.P.RotateOver(DT*wall.angVel);
}

// END code for WallClass ////////////////////////////////////////
////////////////////////////////////////////////////////////////////
