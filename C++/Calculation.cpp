#include "Calculation.h"

////////////////////////////////////////////////////////////////////
// BEGIN code for Calculation /////////////////////////////////////

Calculation::Calculation(InputFile iF, NUMT rmin)
{
	iF.myFind("-calculation-");
	iF.get("timestep");		DT = atof((*iF.location).c_str());
	iF.get("runtime");		runTime = atof((*iF.location).c_str());
	iF.get("numberloops");	numberLoops = atoi((*iF.location).c_str());
	// make a vector of length nrParts, with datasets
	int nrParts;
	iF.myFind("-particles-");
	iF.get("numberparticles");	nrParts = atoi((*iF.location).c_str());
	Vector.resize(nrParts);
	// calculate constraints
	DX_MAX = 0.01*rmin; // DX_MAX is implemented in SearchMethod
	V_MAX = DX_MAX/(3*DT);	// V_MAX is implemented in Part.Move
	nrSpeeding = 0;
	nrTooDeep = 0;
}


////////////////////////////////////////////////////////////////////////////////////////
// Forces due to particle particle interaction
////////////////////////////////////////////////////////////////////////////////////////
void Calculation::Part(Particle &A, Particle &B, NUMT dxN)
{
	// for analysing
	//	if (dxN> dx_max) dx_max = dxN;

////////////////////////////////////////////////////////////////////////////////////////
// force in the normal direction

	// unit vector
	Point AB = B.Get_pos()-A.Get_pos();
	Point N = AB/VectorLength(AB);

	// relative velocity
	NUMT dxDotN = ((A.Get_vel()-B.Get_vel())*N);

	// force	
	/* if vel works in the same direction as N then is dxDotN pos and 
	   the force will increase*/
	NUMT kN = A.Get_kN();//_with(B);
	NUMT dN = A.Get_dN();//_with(B);
	NUMT  fn = kN*dxN + dN*dxDotN;

	Point Fn = -fn*N;

////////////////////////////////////////////////////////////////////////////////////////
// force in tangential direction

	// relative velocity
	Point VelTrans = (A.Get_vel()-B.Get_vel())-(dxDotN*N);
	Point VelRot = VectorProd(A.Get_angVel(),(A.Get_r()*N))-VectorProd(B.Get_angVel(),(-B.Get_r()*N));
	Point DxDotS = VelTrans + VelRot;

	// look for stored info about this contact
	NUMT dxS;
	dataSetIt = Vector[A.Get_pnr()].find(Data(B.Get_pnr(),0,0,0));
	bool prevSlide;
	int prevDir; // to store the original dir of DxDotS
	bool found = (dataSetIt!=Vector[A.Get_pnr()].end());
	if (found)	
	{
		prevSlide = dataSetIt->slide; 
		dxS = dataSetIt->dxTSum;
		prevDir = dataSetIt->direction;
	}
	else 
	{
		dxS = 0; 
		prevSlide = false;
		prevDir = Point::Dir(DxDotS);// so Fs -1*-1 = 1*1 = 1 
	} 

	// make sure dirDxDotS gets the right sign
	// prevDir is the dir of DxDotS at the start of the deformation
	NUMT  dxDotS = VectorLength(DxDotS);
	NUMT dirDxDotS = prevDir*Point::Dir(DxDotS)*dxDotS;
	
	//3/4//if (dxDotS) // else  geen snelheid -> geen tang kracht want statisch evenwicht enkel normkrachten
	//3/4//{// begin vd tang berekeningen

	// the sign of the force depends on the position i.e. the sign of dxS
	NUMT kS = A.Get_kS_with(B);
	NUMT dS = A.Get_dS_with(B);
	NUMT fsprng = kS*dxS + dS*dirDxDotS;
	// coefficient of friction is the smaller one
	NUMT mus = A.Get_mus_with(B); 
	NUMT ffrctn = mus*fabs(fn);// static friction
	bool slide = (fabs(fsprng)>fabs(ffrctn));

	// if it just stopped sliding -> remember direction
	if ((!slide)&&(prevSlide)) 
	{ 
		prevDir =Point::Dir(DxDotS); 
		dataSetIt->direction = prevDir; 
		dirDxDotS = prevDir*Point::Dir(DxDotS)*dxDotS;
	}
	
	// calculate the resulting tangential force
	NUMT fs;
	Point Fs;
	if (dxDotS){
	if(slide)
	{// sliding -> dynamical friction
		// coefficien of friction is the smaller one
		NUMT mud = A.Get_mud_with(B); 
		ffrctn = mud*fabs(fn) ;
		fs = ffrctn;// dynamical friction
		Fs = -fs*DxDotS/dxDotS;
	}
	else
	{// stick -> spring force to model the deformation
		fs = fsprng;		
		Fs = -fs*(DxDotS/dirDxDotS);
	}}/*
	NUMT mud = A.Get_mud_with(B);
	Fs = -mud*fabs(fn)*DxDotS/dxDotS;}*/
	else Fs=ZERO;
	
	// remember to analyse
	//	A.dxN = dxN;  A.dxS = dxS; A.snelhS = DxDotS.y; A.Fs = Fs.y; A.fsprng = fsprng; A.ffrctn = ffrctn;

	// adapt acc
	A.AddForce(Fn + Fs);
	B.AddForce(ZERO - (Fn + Fs));

	// adapt angacc
	A.AddMoment(VectorProd((A.Get_r()*N),Fs));
	// vectorprod(-A,-B) == vectorpord(A,B) so...
	B.AddMoment(VectorProd((B.Get_r()*N),Fs));
	
	// update calculation.Vector[].Data 
	NUMT travelDistance = dirDxDotS*DT;
	if (found) 
	{
		dataSetIt->slide = slide;
		if (slide) 
		{
			dataSetIt->dxTSum = 0;
			dataSetIt->direction = Point::Dir(DxDotS);
		}
		else 
		{
			dataSetIt->dxTSum += travelDistance;
			// direction has allready been changed if necessary
		}
	}
	else Vector[A.Get_pnr()].insert(Data(B.Get_pnr(),slide,travelDistance,Point::Dir(DxDotS)));
	//3/4//}// einde vd tang berekeningen

}


////////////////////////////////////////////////////////////////////////////////////////
// calculation of the forces in particle wall interaction
////////////////////////////////////////////////////////////////////////////////////////
void Calculation::Walls(Particle &A,Point &B,Wall &wall,NUMT dxN,NUMT runTime)
{

////////////////////////////////////////////////////////////////////////////////////////
// force in the normal direction

	// unit vector
	Point AB = B-A.Get_pos();
	Point N = AB/VectorLength(AB);

	// relative velocity
	Point arm = B-wall.Get_R(); // vector from the origin of the wall to the point of contact
								  // = arm for calculating of the momentum
	Point WallVel = wall.VelTrans(runTime)+wall.VelRot(arm); 
	NUMT dxDotN = ((A.Get_vel()-WallVel)*N);

	// force	
	/* if vel works in the same direction as N then is dxDotN pos and 
	   the force will increase*/
	NUMT  fn = A.Get_kN()*dxN + A.Get_dN()*dxDotN;

	Point Fn = -fn*N;

////////////////////////////////////////////////////////////////////////////////////////
// force in tangential direction

	// relative velocity
	Point VelTrans = (A.Get_vel()-WallVel)-(dxDotN*N);
	Point VelRot = VectorProd(A.Get_angVel(),(A.Get_r()*N));
	Point DxDotS = VelTrans + VelRot;

	// look for stored info about this contact
	NUMT dxS;
	dataSetIt = Vector[A.Get_pnr()].find(Data(-wall.Get_wnr(),0,0,0));
	// -wnr because walls are stored under -wnr 
	// (null = walnr 0 and not Get_pnr() 0)
	bool prevSlide;
	int prevDir; // to store the original dir of DxDotS
	bool found = (dataSetIt!=Vector[A.Get_pnr()].end());
	if (found)	
	{
		prevSlide = dataSetIt->slide; // wordt gebruikt om slide op te slaan
		dxS = dataSetIt->dxTSum;
		prevDir = dataSetIt->direction;
	}
	else 
	{
		dxS = 0; 
		prevSlide = false;
		prevDir = Point::Dir(DxDotS);
		// so Fs -1*-1 = 1*1 = 1 
	} 

	// make sure dirDxDotS gets the right sign
	// prevDir is the dir of DxDotS at the start of the deformation
	NUMT  dxDotS = VectorLength(DxDotS);				
	NUMT dirDxDotS = prevDir*Point::Dir(DxDotS)*dxDotS;	

	// the sign of the force depends on the position i.e. the sign of dxS
	NUMT fsprng = A.Get_kS()*dxS + A.Get_dS()*dirDxDotS;
	NUMT ffrctn = A.Get_mus()*fabs(fn);// statische wrijvingscoeff !
	bool slide = (fabs(fsprng)>fabs(ffrctn));

	// if it just stopped sliding -> remember direction
	if ((!slide)&&(prevSlide)) { prevDir =Point::Dir(DxDotS); dataSetIt->direction = prevDir; dirDxDotS = prevDir*Point::Dir(DxDotS)*dxDotS;}
	
	// calculate the resulting tangential force
	NUMT fs;
	Point Fs;
	if (dxDotS){
	if(slide)
	{// sliding -> dynamical friction
		ffrctn = A.Get_mud()*fabs(fn) ;
		fs = ffrctn; // dynamical friction
		Fs = -fs*DxDotS/dxDotS;
	}
	else
	{// stick -> spring force to model the deformation
		fs = fsprng;		
		Fs = -fs*(DxDotS/dirDxDotS);
	}/*
	Fs = -A.Get_mud()*fabs(fn)*DxDotS/dxDotS;*/
	}else Fs = ZERO;

	// adapt acc
	A.AddForce(Fn + Fs);
	
	// hoekversnellingen aanpassen
	A.AddMoment(VectorProd((A.Get_r()*N),Fs));
	
	// update calculation.Vector[].Data
	NUMT travelDistance = dirDxDotS*DT;
	if (found) 
	{
		dataSetIt->slide = slide;
		if (slide) 
		{
			dataSetIt->dxTSum = 0;
			dataSetIt->direction = Point::Dir(DxDotS);
		}
		else 
		{
			dataSetIt->dxTSum += travelDistance;
			// direction has allready been changed if necessary
		}
	}
	else Vector[A.Get_pnr()].insert(Data(-wall.Get_wnr(),slide,travelDistance,Point::Dir(DxDotS)));

}


// END code for Calculation ////////////////////////////////////////
////////////////////////////////////////////////////////////////////
