#include "Output.h"

Output::Output()
{
	ofstream gg("output.txt", ios::out);
	if (!gg){cout<<"Error can't open file 'data' for output.\n";}
}

void Output::SaveData(PartClass &part, Calculation &calculation, WallClass &wall)
{	
	ofstream gg("output.txt",ios::app);
	if (!gg) {cout<<"Error can't open file 'output' for output.\n";}
	gg << " n " << part.Vector.size() <<" "; 
	//gg << calculation.Get_runTime()  <<" ";
	for (part.Iterator = part.Vector.begin(); part.Iterator != part.Vector.end(); 
		part.Iterator++)
		{
			gg	
				//<<(part.Iterator->dxN)  << " "
				//<<(part.Iterator->dxDotN)  << " "
				//<<(part.Iterator->Fn)  << " "
				//<<(part.Iterator->Fs)*(part.Iterator->m)  << " "
				//<<(part.Iterator->dxS)  << " "
				//<<(part.Iterator->dxDotS)  << " "
				//<<(part.Iterator->Fs) << " "
				//<<(part.Iterator->fsprng)  << " "
				//<<(part.Iterator->ffrctn)  << " "
				//<<(part.Iterator->m)  << " "
				//<<(part.Iterator->kN)  << " "
				//<<(part.Iterator->dN)  << " "
				<<(part.Iterator->Get_pos()).x  << " "
				<<(part.Iterator->Get_pos()).y  << " "
				<<(part.Iterator->Get_pos()).z << " "
				//<<(part.Iterator->Get_vel()).x  << " "
				//<<(part.Iterator->Get_vel()).y  << " "
				//<<(part.Iterator->Get_vel()).z  << " "
				//<<wall.Vector[0].a*wall.Vector[0].A.x << " "
				//<<wall.Vector[0].a*wall.Vector[0].A.y << " "
				//<<wall.Vector[0].a*wall.Vector[0].A.z << " "
				/*<<wall.Vector[6].b*wall.Vector[6].B.x << " "
				<<wall.Vector[6].b*wall.Vector[6].B.y << " "
				<<wall.Vector[6].b*wall.Vector[6].B.z << " "
				<<wall.Vector[6].P.x+wall.Vector[6].R.x << " "
				<<wall.Vector[6].P.y+wall.Vector[6].R.y << " "
				<<wall.Vector[6].P.z+wall.Vector[6].R.z << " "*/
				//<<(part.Iterator->ang).x  << " "
				//<<(part.Iterator->ang).y  << " "
				//<<(part.Iterator->ang).z  << " "
				;
		}
	gg	
		//<<calculation.botsingstijd  << " "
		//<<calculation.beginsnelh  << " "
		//<<calculation.restcoef  << " "
		//<<calculation.dx_max  << " "
		
		<<endl;
		
	gg.flush();
	cout<<".";
	cout.flush();
}

void Output::MakeInputFile(PartClass &part, Space &space, Calculation &calculation,
						   WallClass &wall)
{
	{
	std::ofstream outfile("new_input.txt");
	outfile <<"-inputfile-\n";
	outfile <<"\n";
	outfile <<"-particles-"<<"\n";
	outfile	<<"rmin "<<part.Get_RMIN()
			<<" rmax "<<part.Get_RMAX()<<"\n";
	outfile <<"numberparticles "<<part.Vector.size() <<"\n";
	}
	for (part.Iterator = part.Vector.begin(); part.Iterator != part.Vector.end();
         part.Iterator++)
		{
			part.Iterator->MakeInput("new_input.txt");
		 }
	{
	std::ofstream outfile("new_input.txt", ios::app);
	outfile <<"-end-particles-"<<"\n";
	outfile <<"\n";
	outfile <<"-space-"		<<"\n";
	outfile <<"spacex "		<<space.X()
			<<" spacey "	<<space.Y()
			<<" spacez "	<<space.Z()<<"\n";
	outfile <<"-end-space-"<<"\n";
	outfile <<"\n";
	outfile <<"-calculation-"<<"\n";
	outfile <<"timestep "	<<calculation.Get_DT()<<"\n";
	outfile <<"runtime "	<<calculation.Get_runTime()<<"\n";
	outfile <<"numberloops "<<calculation.Get_numberLoops()<<"\n";
	outfile <<"-end-calculation-"<<"\n";
	outfile <<"\n";
	outfile <<"-walls-"<<"\n";
	outfile <<"numberwalls "<<wall.Vector.size()<<"\n";
	}
	for (wall.Iterator = wall.Vector.begin(); wall.Iterator != wall.Vector.end();
         wall.Iterator++)
		{
			wall.Iterator->MakeInput("new_input.txt");
		 }
	{
	std::ofstream outfile("new_input.txt", ios::app);
	outfile <<"-end-walls-"<<"\n";
	outfile <<"\n";
	outfile <<"-end-inputfile-"<<"\n";
	outfile.flush();
	}
}

