//////////////////////////////////////////////////////////////////
#ifndef INPUTFILE_H												//
#define INPUTFILE_H												//
// begin of InputFile.h											//
//////////////////////////////////////////////////////////////////

#include "General.h"
#include <algorithm>// for Find
#include <fstream>	// for inlezen file
#include <string>	// for string
#include <vector>	// for vector

struct Scope
{
	std::string *begin;
	std::string *end;
};

typedef std::vector<std::string> STRINGVECTOR;

class InputFile
{
public:  
	InputFile();
	void myFind(std::string str);
	void get(std::string str);
	std::string				*location;		
private:
	Scope					scope;
	STRINGVECTOR			text;
	STRINGVECTOR::iterator	textIt;
};

//////////////////////////////////////////////////////////////////
#endif															//
// end of InputFile.h											//
//////////////////////////////////////////////////////////////////