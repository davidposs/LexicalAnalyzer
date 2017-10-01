/******************************************************************************
* File:		Token.h
* Authors:	David Poss, Douglas Galm
*
* Usage:	Used to create tokens of each type, and is responsible for running 
*			each lexeme through the appropriate FSM, changing types when 
*			necessary.
*
******************************************************************************/

#ifndef _TOKEN_H_
#define _TOKEN_H_
#include "globals.h"

class Token {
public:
	/* Constructors */
	Token();
	Token(int state, bool accept, std::string token, TokenType type);
	
	/* Returns the state the token is in according to its FSM */
	int getState();
	
	/* Returns token type, i.e integer, real, or identifier */
	TokenType getType();

	/* Returns the actual value of the token*/
	std::string getToken();
	
	/* Sets all data in class to default values */
	void clear();

	/* Send an input string through the appropriate FSM transition table */
	void updateState(std::string input);
	
	/* Manually change the state. Useful when switching from Integer to Real */
	void updateStateManually(int state);

	/* Function to run an Integer through the real FSM if we find a '.' */
	void runLexemeThroughReals();

	/* Change token type to a new type */
	void updateType(TokenType type);

	/* Add value to end of current token */
	void updateToken(std::string val);

	/* Each of these return whether or not the token is what they check for */
	bool isReal();
	bool isInteger();
	bool isAccepted();
	bool isIdentifier();

	/* Handy output function */
	friend std::ostream& operator<<(std::ostream& os, const Token& t);

private:
	/* Stores the token's state in the FSM*/
	int			state_;
	/* Used to know if token is in an accepting state or not */
	bool		accept_;
	/* Stores token type */
	TokenType	type_;
	/* Stores value of token */
	std::string token_;
};

#endif