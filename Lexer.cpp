/******************************************************************************
* File:    Lexer.cpp
* Authors: David Poss, Douglas Galm
*
* Usage:   Creates a Lexer that can parse a specified file, dividing it up into
*          tokens with the getTokens(...) function, as well as add them into 
*          one of two lists, depending on if they are accepted or rejected by
*          their appropriate FSM.
*
******************************************************************************/

#include "Lexer.h"
#include "Helpers.h"
#include <iostream>
Lexer::Lexer() : lexemes_(std::list<Pair>()), unknowns_(std::list<Pair>()) {}

TokenType Lexer::strToTokenType(std::string str) {
	if (isAlphaOrPound(str)) {
		return Identifier;
	}
	/* Assume digit until '.' for real is found */
	else if (isDigit(str)) {
		return Integer;
	}
	else if (str == ".") {
		return Real;
	}
	else  {
		return Unknown;
	}
}
/* Function to add a lexeme to the list */
void Lexer::addLexeme(Token& lexeme) {
	if (!lexeme.isAccepted()) {
		unknowns_.push_back(Pair(lexeme.getToken(), 
			tokenTypeToString(lexeme.getType())));
		lexeme.clear();
		return;
	}
	switch (lexeme.getType()) {
	case(Integer):
		lexemes_.push_back(Pair(lexeme.getToken(), "integer"));
		break;
	case(Real):
		lexemes_.push_back(Pair(lexeme.getToken(), "real"));
		break;
	case(Identifier):
		lexemes_.push_back(Pair(lexeme.getToken(), "identifier"));
		break;
	case(Keyword):
		lexemes_.push_back(Pair(lexeme.getToken(), "keyword"));
		break;
	case(Operator):
		lexemes_.push_back(Pair(lexeme.getToken(), "operator"));
		break;
	case(Separator):
		lexemes_.push_back(Pair(sepToString(lexeme.getToken()), "separator"));
		break;
	case(Unknown):
		unknowns_.push_back(Pair(lexeme.getToken(), "Unknown"));
		break;
	default:
		lexeme.clear();
		return;
	}
	lexeme.clear();
}

/* Main lexer function */
void Lexer::getTokens(std::string inputFile) {
	/* Read character from file one at a time, until eof */
	char c;
	Token currentLexeme;
	std::fstream inputCode(inputFile);
	if (!inputCode.is_open()) {
		std::cout << "Error opening file. Please rename it to 'sample.txt'"
			<< std::endl << "Press enter to continue. " << std::endl;
		std::cin.get();
		return;
	}
	while (inputCode.get(c)) {
		std::string input = std::string(1, c);
		/* See if input terminates a token */

		if (isOfType(separators, input, NUM_SEPARATORS)
			|| isOfType(operators, input, NUM_OPERATORS)
			|| isOfType(miscellaneous, input, NUM_MISCELLANEOUS)
			|| isSpace(input))	
		{

			if (isOfType(keywords, currentLexeme.getToken(), NUM_KEYWORDS)) {
				currentLexeme.updateType(Keyword);
				addLexeme(currentLexeme);
				inputCode.unget();
			}
			else if (isSpace(input) && currentLexeme.getToken() != "") {
				addLexeme(currentLexeme);
				inputCode.unget();
			}
			else if (currentLexeme.isIdentifier()) {
				addLexeme(currentLexeme);
				inputCode.unget();
			}
			else if (currentLexeme.isInteger()) {
				addLexeme(currentLexeme);
				inputCode.unget();
			}
			else if (currentLexeme.isReal()) {
				addLexeme(currentLexeme);
				inputCode.unget();
			}
			else if (isOfType(separators, input, NUM_SEPARATORS)) {
				if (input == ":") {
					char temp;
					inputCode.get(temp);
					std::string nextInput = std::string(1, temp);
					if (input + nextInput == ":=") {
						Token t(0, true, input + nextInput, Operator);
						addLexeme(t);
						continue;
					}
					else {
						inputCode.unget();
					}
				}
				Token t(0, true, input, Separator);
				addLexeme(t);
			}
			else if (isOfType(operators, input, NUM_OPERATORS)) {
				/* See if next token is also an operator, like <= */
				char temp;
				inputCode.get(temp);
				std::string nextInput = std::string(1, temp);
				if (isOfType(operators, input + nextInput, NUM_OPERATORS)) {
					Token t(0, true, input + nextInput, Operator);
					addLexeme(t);
				}
				else {
					inputCode.unget();
					Token t(0, true, input, Operator);
					addLexeme(t);
				}
			}
			else if (isOfType(miscellaneous, input, NUM_MISCELLANEOUS)) {
				if (input == "%") {
					char temp;
					inputCode.get(temp);
					std::string nextInput = std::string(1, temp);
					if (input + nextInput == "%%") {
						Token t(0, true, input + nextInput, Separator);
						addLexeme(t);
						continue;
					}
					else {
						inputCode.unget();
						Token t(0, true, input, Unknown);
						addLexeme(t);
					}
				}
				else {
					Token t(0, true, input, Unknown);
					addLexeme(t);
				}
			}
			currentLexeme.clear();
		}
		else { /* input doesn't terminate a token */
			if (currentLexeme.getToken().length() == 0) {
				currentLexeme.updateToken(input);
				currentLexeme.updateType(strToTokenType(input));
				if (input == ".") {
					currentLexeme.updateType(Real);
				}
				currentLexeme.updateState(input);
				continue;
			}
			else if (currentLexeme.isInteger() && isDigit(input)) {
				currentLexeme.updateToken(input);
				currentLexeme.updateState(input);
			}
			else if (currentLexeme.isReal() && isDigit(input)) {
				currentLexeme.updateToken(input);
				currentLexeme.updateState(input);
			}
			else if (currentLexeme.isReal() && input == ".") {
				currentLexeme.updateToken(input);
				currentLexeme.updateState(input);
			}
			else if (currentLexeme.isInteger() && input == ".") {
				currentLexeme.updateToken(input);
				currentLexeme.updateType(Real);
				currentLexeme.updateStateManually(0);
				/* Switched from Integer to real, so run current token 
				   through the real machine FSM to get the correct state*/
				currentLexeme.runLexemeThroughReals();
			}
			else if (currentLexeme.isIdentifier()) {
				currentLexeme.updateToken(input);
				currentLexeme.updateType(Identifier);
				currentLexeme.updateState(input);
			}
		}
	} /* end for loop */
	inputCode.close();
} /* end function */

void Lexer::printLexemes(std::string filename) {
	std::cout << "Printing lexemes to " << filename << std::endl;
	std::ofstream output;
	output.open(filename);
	if (!output.is_open()) {
		std::cout << "Error opening file. Please rename it to 'sample.txt'";
		std::cout << std::endl;
		std::cin.get();
		return;
	}
	output << "Accepted Tokens:" << std::endl;
	std::list<Pair>::iterator it;
	for (it = lexemes_.begin(); it != lexemes_.end(); it++) {
		output << *it << std::endl;
	}
	if (unknowns_.size() > 0) {
		output << std::endl << "Failed Tokens:" << std::endl;
		for (it = unknowns_.begin(); it != unknowns_.end(); it++) {
			output << *it << std::endl;
		}
	}
	output.close();
}