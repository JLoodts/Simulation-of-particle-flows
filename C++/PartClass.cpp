#include "PartClass.h"

////////////////////////////////////////////////////////////////////
// BEGIN code for PartClass ////////////////////////////////////////
Particle::Particle(InputFile &iF)
{

	iF.get("pnr");	pnr	= atoi((*iF.location).c_str());
	iF.get("m");	m		= atof((*iF.location).c_str()); 
	iF.get("r");	r		= atof((*iF.location).c_str());
	iF.get("kn");	kN		= atof((*iF.location).c_str());
	iF.get("ks");	kS		= atof((*iF.location).c_str());
	iF.get("dn");	dN		= atof((*iF.location).c_str());
	iF.get("ds");	dS		= atof((*iF.location).c_str());
	iF.get("mud");	mud	= atof((*iF.location).c_str());
	iF.get("mus");	mus	= atof((*iF.location).c_str());
	iF.get("posx");	pos.x	= atof((*iF.location).c_str());
	iF.get("posy");	pos.y	= atof((*iF.location).c_str());
	iF.get("posz");	pos.z	= atof((*iF.location).c_str()); 
	iF.get("velx");	vel.x	= atof((*iF.location).c_str());
	iF.get("vely");	vel.y	= atof((*iF.location).c_str());
	iF.get("velz");	vel.z	= atof((*iF.location).c_str()); 
	iF.get("angx");	ang.x	= atof((*iF.location).c_str());
	iF.get("angy");	ang.y	= atof((*iF.location).c_str());
	iF.get("angz");	ang.z	= atof((*iF.location).c_str());
	iF.get("angvelx");	angVel.x	= atof((*iF.location).c_str());
	iF.get("angvely");	angVel.y	= atof((*iF.location).c_str());
	iF.get("angvelz");	angVel.z	= atof((*iF.location).c_str());
	angm = 0.4*m*r*r;
	dead = false;
	prevPos = pos;
}

bool Particle::OverAccelerating(NUMT V_MAX, NUMT DT)
{
	NUMT acc_value = VectorLength(acc);
		if ( acc_value> V_MAX/DT) 
		{
			//acc = (V_MAX/DT)*acc/acc_value;
			return true; 
		}
	else return false;
}

PartClass::PartClass(InputFile iF)
{
	iF.myFind("-particles-");
	iF.get("rmin");
	RMIN = atof((*iF.location).c_str());
	iF.get("rmax");
	RMAX = atof((*iF.location).c_str());
	iF.get("numberparticles");
	numParts = atoi((*iF.location).c_str());// number particles
	for (int i=0; i<numParts; ++i)
	{
		Particle part(iF);
		Vector.push_back(part);
	}
	Clear();
}


void PartClass::Clear()
/*	
	this function sets the accelerations to ZERO 
	and removes the dead particles
*/
{
	for (Iterator = Vector.begin(); Iterator != Vector.end();
         Iterator++)
	if (Iterator->Dead()) Vector.erase(Iterator--);
	else Iterator->Clear();
}


void PartClass::Move(Calculation &calculation)		
{	
	Point Drag(0,0,0);
	NUMT v=0, r=0;
	NUMT DT = calculation.Get_DT();
	for (Iterator = Vector.begin(); Iterator != Vector.end();
         Iterator++)
	{		// incorporate the air resistance
			// Caccent = 0.5*airDensity*dragcoeff*pi=0.5*1.2*0.44*pi=0.8293804
			 v = VectorLength(Iterator->Get_vel());
			 r = Iterator->Get_r();
			Drag = (-0.8293804*r*r*v*v)*(Iterator->Get_vel()/v);
			Iterator->AddForce(Drag);
			if (Iterator->OverAccelerating(calculation.Get_V_MAX(),calculation.Get_DT()))calculation.Increment_nrSpeeding();
			Iterator->AddVel(DT);
			Iterator->AddPos(DT);
			Iterator->AddAngVel(DT);
			// perhaps keep ang between 0 and 2 PI 
			Iterator->AddAng(DT); 
			Iterator->SetPrevPos();
	}
	Clear();
}


void Particle::MakeInput(const char *fileName)
{
	std::ofstream outfile(fileName, ios::app);
	outfile	<<"pnr "	<<pnr
			<<" m "		<<m
			<<" r "		<<r
			<<" kn "	<<kN
			<<" ks "	<<kS
			<<" dn "	<<dN
			<<" ds "	<<dS
			<<" mud "	<<mud
			<<" mus "	<<mus
			<<" posx "	<<pos.x
			<<" posy "	<<pos.y
			<<" posz "	<<pos.z
			<<" velx "	<<vel.x
			<<" vely "	<<vel.y
			<<" velz "	<<vel.z
			<<" angx "	<<ang.x
			<<" angy "	<<ang.y
			<<" angz "	<<ang.z
			<<" angvelx "<<angVel.x
			<<" angvely "<<angVel.y
			<<" angvelz "<<angVel.z
			<<"\n";
}

// END code of PartClass ////////////////////////////////////////
////////////////////////////////////////////////////////////////////

