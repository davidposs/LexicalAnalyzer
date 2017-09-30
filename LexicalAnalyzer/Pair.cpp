/******************************************************************************
* File:    Pair.cpp
* Authors: David Poss, Douglas Galm
*
* Usage:   To create lexeme and token-type pairs that can be easily printed,
*          modified, and inserted into a list.
* 
******************************************************************************/
#pragma once
#include "Pair.h"

Pair::Pair() : val1(""), val2("") {}
Pair::Pair(std::string l, std::string t) : val1(l), val2(t) {}

std::ostream& operator<<(std::ostream& os, Pair P) {
	os << "Token: " << P.val1 << "\t\t Type: " << P.val2;
	return os;
}
