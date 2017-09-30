/******************************************************************************
* File:		Helpers.cpp
* Authors:	David Poss, Douglas Galm 
* 
* Usage:	Has a good amount of helper functions that make life just a little
*			bit easier
* 
******************************************************************************/

#include "globals.h"

bool isOfType(const std::string* type, std::string lexeme, size_t size) {
	for (size_t i = 0; i < size; i++) {
		if (type[i] == lexeme) {
			return true;
		}
	}
	return false;
}

bool isSpace(std::string str) {
	return str == " ";
}
bool isDigit(std::string str) {
	return isdigit(str[0]);
}
bool isAlphaOrPound(std::string str) {
	return ((isalpha(str[0])) || str[0] == '#');
}
std::string sepToString(std::string separator) {
	if (separator == "\n") {
		return "\\n";
	}
	if (separator == "\t") {
		return "\\t";
	}
	if (separator == " ") {
		return "<space>";
	}
	else return separator;
}

std::string tokenTypeToString(TokenType t) {
	return Tokens[t - 1];
}
