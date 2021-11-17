#include "InputFile.h"

////////////////////////////////////////////////////////////////////
// BEGIN code for InputFile ///////////////////////////////////////
InputFile::InputFile()
{
	std::ifstream infile("input.txt");
	std::istream_iterator<std::string> ifile(infile);
	std::istream_iterator<std::string> eos;

	std::copy(ifile, eos, std::inserter(text, text.begin()));
	scope.begin = text.begin();
	scope.end	= text.end();   
}

void InputFile::myFind(std::string str1)

	/* checks wether string str1 is part of text
	if so, then the scope is put on that section
	if not, the scope is kept on the entire text
	*/
{
	location = std::find(text.begin(), text.end(), str1);
	if (*location == str1) 
	{
		scope.begin = location;
		std::string str2("-end");
		str2.append(str1); 
		std::string *endLocation;
		endLocation = std::find(scope.begin, text.end(), str2);
		if (*endLocation == str2) scope.end = endLocation;
		else ::cout <<"Error in determining the scope for "<<str1.c_str()
					<<" in inputfile."<<endl;
	}
	else
	{
		::cout	<<"Error in searching for "<<str1.c_str()<<" in inputfile."<<endl;
		scope.begin = text.begin();
		scope.end	= text.end();
	}
}

void InputFile::get(std::string str)
{
	location = std::find(scope.begin, scope.end, str);
	if (*location == str) 
	{
		location++;
		scope.begin = location;
	}
	else ::cout <<"Error in retrieving "<<str.c_str()<<" in inputfile."<<endl;
}





// END code of InputFile ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////