//////////////////////////////////////////////////////////////////
#ifndef PARTICLE_H												//
#define PARTICLE_H												//
// begin of Particle.h											//
//////////////////////////////////////////////////////////////////

#include "General.h"

class Particle
{	
public:
	Particle() {}
	Particle(int  newpnr,	NUMT newm, NUMT newr, NUMT newkN, NUMT newkS,NUMT newdN, NUMT newdS,
			 NUMT newmud, NUMT newmus, Point newpos,	Point newvel, Point newang, Point newangVel)
			:pnr(newpnr),	m(newm), angm(0.4*newm*newr*newr), r(newr), kN(newkN),kS(newkS), dN(newdN),dS(newdS),
			 mud(newmud), mus(newmus), pos(newpos),	vel(newvel), ang(newang), angVel(newangVel)
			 { dead = false; prevPos = pos;}
	Particle(InputFile &iF);	
	int	Get_pnr()					{return pnr;}
	NUMT Get_kN()					{return kN;}
	NUMT Get_dN()					{return dN;}
	NUMT Get_kS()					{return kS;}
	NUMT Get_dS()					{return dS;}
	NUMT Get_mus()					{return mus;}
	NUMT Get_mud()					{return mud;}
	NUMT Get_m()					{return m;}
	NUMT Get_r()					{return r;}
	NUMT Get_kN_with(Particle &B)	{return (kN*m + B.kN*B.m) / (m+B.m);}
	NUMT Get_dN_with(Particle &B)	{return (dN*m + B.dN*B.m) / (m+B.m);}
	NUMT Get_kS_with(Particle &B)	{return (kS*m + B.kS*B.m) / (m+B.m);}
	NUMT Get_dS_with(Particle &B)	{return (dS*m + B.dS*B.m) / (m+B.m);}
	NUMT Get_mus_with(Particle &B)	{if (mus>B.mus) return B.mus; else return mus;}
	NUMT Get_mud_with(Particle &B)	{if (mud>B.mud) return B.mud; else return mud;}
	Point Get_pos()					{return pos;}
	Point Get_prevPos()				{return prevPos;}
	Point Get_vel()					{return vel;}
	Point Get_angVel()				{return angVel;}
	void AddMoment(Point Moment)	{angAcc = angAcc + Moment / angm;}
	void AddForce(Point Force)		{acc = acc + Force / m;}
	void AddVel(NUMT DT)			{vel = vel + acc*DT;}
	void AddPos(NUMT DT)			{pos = pos + vel*DT;}
	void AddAngVel(NUMT DT)			{angVel = angVel + angAcc*DT;}
	void AddAng(NUMT DT)			{ang = ang + angVel*DT;}
	void Clear()					{acc = Point(0,0,GRAVITY); angAcc = ZERO;}
	void MakeInput(const char *fileName);
	void RemoveThis()				{dead = true;}
	void SetPrevPos()				{prevPos = pos;}
	bool OverAccelerating(NUMT V_MAX, NUMT DT);
	bool Dead()						{return dead;}

	// overloaded inequality operator
	bool operator!=(const Particle & B) const{return (pnr != B.pnr);}
	// overloeded equality operator
	bool operator==(const Particle & B) const{return (pnr == B.pnr);}
private:
// general
	int		pnr;	// number of the particle
// physical properties
	NUMT	m,angm,	// mass
			kN,kS,	// stifness = springconstant
			dN,dS,	// damping constant
			mud,	// constant of dynamical friction
			mus;	// constant of static friction
// kinetics
	Point	acc,	// acceleration 
			angAcc,	// angular acceleration
			pos,	// position
			ang,	// angle
			vel,	// velocity
			angVel;	// angular velocity
// geometry
	NUMT	r;		// radius
// to know when to remove the particle
	bool	dead;
	Point	prevPos;// previous position for an easy update in SearchMethod
};

//////////////////////////////////////////////////////////////////
#endif															//
// end of Particle.h											//
//////////////////////////////////////////////////////////////////