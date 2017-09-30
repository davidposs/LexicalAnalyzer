/******************************************************************************
* Lexical Analyzer
* David Poss, Douglas Galm
* CPSC 323
* Professor Choi
*
* Notes: see README for information on compiling and running. Should work on
* both linux and windows systems.
*
******************************************************************************/

#include "Token.h"
#include "Lexer.h"

int main() {

	std::string input = "sample.txt";
	std::string output = "finished.txt";

	Lexer lexer;
	lexer.getTokens(input);
	lexer.printLexemes(output);

	return 0;
}