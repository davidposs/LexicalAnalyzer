/******************************************************************************
* File:		Helpers.h
* Authors:	David Poss, Douglas Galm
*
* Usage:	Defines several helper functions for better quality of life and
*			readability. Nothing too fancy here.
*
******************************************************************************/

#pragma once
#ifndef _HELPERS__H_
#define _HELPERS__H_

#include "globals.h"
/* Searches a string array for a lexeme, returns true if found */
bool isOfType(const std::string* type, std::string lexeme, size_t size);

/* Returns true if string passed is a space character */
bool isSpace(std::string str);

/* Returns true if the string passed is a digit */
bool isDigit(std::string str);

/* Returns true if the string passed is a letter or # symbol */
bool isAlphaOrPound(std::string str);

/* Converts separator characters to printable strings */
std::string sepToString(std::string separator);

/* Converts a TokenType enum value to a printable string */
std::string tokenTypeToString(TokenType t);

#endif