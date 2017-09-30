/******************************************************************************
* File:		Helpers.cpp
* Authors:	David Poss, Douglas Galm
*
* Usage:	Defines several global variables, including lists of keywords, 
*			separators, operators, miscellaneous characters, and token types. 
*			Also includes the transition tables for each FSM as well as their 
*			accepted states.
*
******************************************************************************/

#pragma once
#include <string>
#ifndef _GLOBALS_H_
#define _GLOBALS_H_

enum TokenType {
	Integer		= 1,
	Real		= 2,
	Identifier	= 3,
	Operator	= 4,
	Keyword		= 5,
	Separator	= 6,
	Unknown		= 7
};

#define NUM_KEYWORDS 13
const std::string keywords[NUM_KEYWORDS] = {
	"int",			"if",			"else",			"fi",			"while",
	"return",		"read",			"write",		"integer",		"boolean",
	"false",		"true",			"floating" };

#define NUM_SEPARATORS 13
const std::string separators[NUM_SEPARATORS] = {
	"[",		"]",		"{",		"}",		"\n",
	"(",		")",		",",		";",		"@",
	":",		"%%",		"\t" };

#define NUM_OPERATORS 11
const std::string operators[NUM_OPERATORS] = {
	"=",		"+",		"-",		"=",		">=",		"/="		
	"/",		">",		"<",		"*",		"<=" };

#define NUM_MISCELLANEOUS 12
const std::string miscellaneous[NUM_MISCELLANEOUS] = {
	"`",		"~",		"!",		"$",		"%",		"&",
	"^",		"_",		"|",		"\\",		"?",		","
};

#define NUM_TOKENS 7
const std::string Tokens[NUM_TOKENS] = {
	"integer",			"real",			"identifier",		"unkown"
	"operator",			"keyword",		"separator" };

/* Transition tables */
const int idTable[4][2] = {
	/* Inputs		l	 #      /*
	/* State 0 */	1,	-1,		/* start with # */
	/* State 1 */	1,	 2,
	/* State 2 */	3,	-1,		/* no consecutive #s */
	/* State 3 */	3,	 2,
	/* -1 = empty set			*/
};
const int idAcceptStates[] = { 1, 3 };

const int intTable[2][1] = {
	/* Inputs		d */
	/* State 1 */	1,
	/* State 2 */	1
};
const int intAcceptStates[] = { 1 };

const int realTable[4][2] = {
	/* Inputs		d	'.'		*/
	/* State 0 */	1,	-1,
	/* State 1 */	1,	 2,
	/* State 2 */	3,	-1,
	/* State 3 */	3,	-1
	/* -1 = empty set			*/
};
const int realAcceptStates[] = { 3 };

#endif