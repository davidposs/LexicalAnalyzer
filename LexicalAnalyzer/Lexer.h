/******************************************************************************
* File:		Lexer.h
* Authors:	David Poss, Douglas Galm
*
* Usage:	Declares Lexer class. This and the Token class do the heavy lifting 
*			for the	lexical analysis. 
*			Keeps two lists: one for accepted tokens, and one for rejected ones
*			that either don't belong to any token type we know of yet, or don't
*			have any meaning in the language, e.g '~'
*
******************************************************************************/

#ifndef _LEXER_H_
#define _LEXER_H_
#pragma once
#include "Pair.h"
#include <list>
#include "Token.h"

class Lexer {
public:
	Lexer();

	/* Returns possible TokenType based on value of input string */
	TokenType charToTokenType(std::string str);

	/* Function to add a lexeme to the appropriate list */
	void addLexeme(Token& lexeme);

	/*	Main lexer function : grabs tokens from a specified file and runs them
	*   through a FSM until they are either accepted, or rejected. */
	void getTokens(std::string inputFile);

	/*  Print the lexeme - token type pairs to a file*/
	void printLexemes(std::string filename);

private:
	std::list<Pair> lexemes_;
	std::list<Pair> unknowns_;
};

#endif