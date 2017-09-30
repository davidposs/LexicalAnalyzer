/******************************************************************************
* File:		Pair.h
* Authors:	David Poss, Douglas Galm
*
* Usage:	Used to create list of lexeme:type pairs for easier output and 
*			management. 
* 
******************************************************************************/

#ifndef _PAIR_H_
#define _PAIR_H_

#include <string>
#include <fstream>

class Pair {
public:
	Pair();
	Pair(std::string l, std::string t);

	friend std::ostream& operator<<(std::ostream& os, Pair P);
private:
	std::string val1, val2;
};

#endif