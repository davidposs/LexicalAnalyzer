/******************************************************************************
* File:		Tokens.cpp
* Authors:	David Poss, Douglas Galm
*
* Usage:	Implements the functionalities of the Token class. See the Tokens.h 
*			file for more information about specific methods.
*
******************************************************************************/

#include "Token.h"
#include "Helpers.h"
#include <iostream>

Token::Token() : state_(0), accept_(false), token_(""), type_(Unknown) { }

Token::Token(int state, bool accept, std::string token, TokenType type)
	: state_(state), accept_(accept), token_(token), type_(type) { }

void Token::updateType(TokenType type) {
	type_ = type;
}

void Token::updateToken(std::string val) {
	token_ += val;
}

bool Token::isReal() {
	return (type_ == Real);
}

bool Token::isInteger() {
	return (type_ == Integer);
}

bool Token::isIdentifier() {
	return (type_ == Identifier);
}

void Token::clear() {
	token_ = "";
	type_ = Unknown;
	state_ = 0;
	accept_ = false;
}

void Token::updateState(std::string input) {
	/* Integer */
	if (type_ == Integer && isDigit(input)) {
		state_ = intAcceptStates[0];
		accept_ = true;
	}
	/* Real */
	else if (type_ == Real) {
		if (state_ >= 0) {
			if (input == ".") {
				state_ = realTable[state_][1];
			}
			else if (isDigit(input)) {
				state_ = realTable[state_][0];
			}
			accept_ = (state_ == realAcceptStates[0]);
		}
		else {
			accept_ = false;
		}
	}
	/* Identifier */
	if (type_ == Identifier) {
		if (state_ >= 0 && isAlphaOrPound(input)) {
			if (input == "#") {
				state_ = idTable[state_][1];
			}
			else if (isalpha(input[0])) {
				state_ = idTable[state_][0];
			}
			accept_ = (state_ == idAcceptStates[0]
				|| state_ == idAcceptStates[1]);
		}
		else {
			accept_ = false;
		}
	}
} /* end updateState */

void Token::updateStateManually(int state) {
	state_ = state;
}

void Token::runLexemeThroughReals() {
	std::string temp = getToken();
	for (size_t j = 0; j < temp.length(); j++) {
		updateState(std::string(1, temp[j]));
	}
}

std::ostream& operator<<(std::ostream& os, const Token& t) {
	os << t.token_ << "\t\t" << t.type_ << std::endl;
	return os;
}

bool Token::isAccepted() { 
	return accept_; 
}

TokenType Token::getType() { 
	return type_; 
}

std::string Token::getToken() { 
	return token_; 
}

int Token::getState() { 
	return state_; 
}