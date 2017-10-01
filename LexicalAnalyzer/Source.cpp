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
#include "Helpers.h"
#include "Lexer.h"
#include <iostream>
int main() {
	std::string input;
	std::string output;

	while (true) {
		std::cout << "Enter a file to convert or hit Ctrl+c to quit\nFile: ";
		std::cin >> input;

		Lexer lexer;
		if (lexer.getTokens(input)) {
			lexer.printLexemes(converToOutputName(input));
		}
		std::cin.clear();
	}
	return 0;
}