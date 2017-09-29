/******************************************************************************
* Lexical Analyzer
* David Poss, Douglas Galm
* CPSC 323
* Professor Choi
******************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <string>
#include <iterator>
#include <fstream>
#include <list>

#define ERROR_STATE -1

#define NUM_KEYWORDS 13
std::string keywords[NUM_KEYWORDS] = {
	"int",		"if",		"else",		"fi",		"while",
	"return",	"read",		"write",	"integer",	"boolean",
	"false",	"true",		"floating" };

#define NUM_SEPARATORS 13
std::string separators[NUM_SEPARATORS] = {
	"[",		"]",		"{",		"}",
	"(",		")",		",",		";",
	"@",		":",		"%%",		"\n",		"\t" };

#define NUM_OPERATORS 11
std::string operators[NUM_OPERATORS] = {
	"=",		"+",		"-",		"=",
	"*",		"/",		"/=",		">",
	"<",		"=>",		"<=" };

#define NUM_TOKENS 6
std::string Tokens[NUM_TOKENS] = {
	"integer",		"real",		"identifier",
	"operator",		"keyword",	"separator" };

/* Transition tables */
const int idTable[4][2] = {
	/* Inputs		l	 #       /*
	/* State 0 */	2,	-1,
	/* State 1 */	2,	 3,
	/* State 2 */	4,	-1,
	/* State 3 */	4,	 3,
	/* -1 = empty set			*/
};
const int idAcceptStates[] = { idTable[2][0], idTable[3][0] };

const int intTable[2][1] = {
	/* Inputs		d */
	/* State 1 */	2,
	/* State 2 */	2
};
const int intAcceptStates[] = { 2 };

const int realTable[4][2] = {
	/* Inputs		d	'.'		*/
	/* State 1 */	2,	-1,
	/* State 2 */	2,	 3,
	/* State 3 */	4,	-1,
	/* State 4 */	4,	-1
	/* -1 = empty set			*/
};
const int realAcceptStates[] = { 4 };

bool isKeyword(std::string lexeme) {
	for (int i = 0; i < NUM_KEYWORDS; i++) {
		if (keywords[i] == lexeme) {
			return true;
		}
	}
	return false;
}
bool isSeparator(std::string str) {
	for (int i = 0; i < NUM_SEPARATORS; i++) {
		if (separators[i] == str) {
			return true;
		}
	}
	return false;
}
bool isOperator(std::string str) {
	for (int i = 0; i < NUM_OPERATORS; i++) {
		if (operators[i] == str) {
			return true;
		}
	}
	return false;
}
bool isSpace(std::string str) {
	return str == " ";
}
bool isDigit(std::string str) {
	for (size_t i = 0; i < str.length(); i++) {
		if (!isdigit(str[i])) {
			return false;
		}
	}
	return true;
}
bool isAlphaOrPound(std::string str) {
	return ((isalpha(str[0])) || str[0] == '#');
}

enum TokenType {
	Integer = 1,
	Real = 2,
	Identifier = 3,
	Operator = 4,
	Keyword = 5,
	Separator = 6,
	Unknown = 7
};

class Token {
public:
	Token() : state_(0), accept_(false), token_(""), type_(Unknown) { }

	Token(int state, bool accept, std::string token, TokenType type)
		: state_(state), accept_(accept), token_(token), type_(type) { }

	void updateType(TokenType type) { type_ = type; }
	void updateToken(std::string val) { token_ += val; }

	bool isReal() { return (type_ == Real); }
	bool isInteger() { return (type_ == Integer); }
	bool isIdentifier() { return (type_ == Identifier); }


	void clear() {
		token_ = "";
		type_ = Integer;
		state_ = 0;
		accept_ = false;
	}

	void updateState(std::string input) {
		if (type_ == Integer && isDigit(input)) {
			/* No lookup needed, integers are always in a final state */
			state_ = 2;
			accept_ = true;
		}
		if (type_ == Real && (isDigit(input) || input == ".")) {
			if (input == ".") { state_ = realTable[state_ - 1][1]; }
			else { state_ = realTable[state_ - 1][0]; }
			accept_ = (state_ == realAcceptStates[0]);
		}

		if (type_ == Identifier && isAlphaOrPound(input)) {
			if (input == "#") { state_ = idTable[state_ - 1][1]; }
			else { state_ = idTable[state_ - 1][0]; }
			accept_ = (state_ == idAcceptStates[0] || state_ == idAcceptStates[1]);
		}
	} /* end updateState */


	friend std::ostream& operator<<(std::ostream& os, const Token& t) {
		os << t.token_ << "\t\t" << t.type_ << std::endl;
		return os;
	}

	bool isAccepted() { return accept_; }
	TokenType getType() { return type_; }
	std::string getToken() { return token_; }

	/* TODO
	Add function to see if Token is in an accept state according to table_
	Add function to pass all of current token through a machine
	*/


private:
	int			state_;
	bool		accept_;
	TokenType	type_;
	std::string token_;
};

struct Pair {
	Pair() : val1("empty"), val2("empty") {}
	Pair(std::string l, std::string t) : val1(l), val2(t) {}

	std::string val1;
	std::string val2;

	friend std::ostream& operator<<(std::ostream& os, Pair P) {
		os << "Token: " << P.val1 << "\t\t Type: " << P.val2;
		return os;
	}
};

class Lexer {
public:
	Lexer() : lexemes_(std::list<Pair>()) {}

	TokenType charToTokenType(std::string str) {
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
		else {
			return Identifier;
		}
	}
	/* Function to add a lexeme to the list */
	void addLexeme(Token lexeme) {
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
			lexemes_.push_back(Pair(lexeme.getToken(), "separator"));
			break;
		default:
			return;
		}
		return;
	}



	/* Main lexer function */
	void getTokens(std::string inputCode) {
		/* Read character from file one at a time, until eof
		std::fstream inputFile;
		inputFile.open(filename, 'r');
		while (inputFile.get(c)) { */

		Token currentLexeme;

		for (std::string::iterator i = inputCode.begin(); i != inputCode.end(); i++)
		{
			std::string input = std::string(1, *i);
			if (currentLexeme.getType() == Integer && charToTokenType(input) == Real) {
				// We changed token types from integer to real
				// feed currentToken through real FSM 
				currentLexeme.updateType(Real);
			}

			// See if input terminates a token
			if (isSeparator(input) || isOperator(input) || isSpace(input)) {
				std::cout << "End of lexeme: " << currentLexeme.getToken() << std::endl;
				if (isKeyword(currentLexeme.getToken())) {
					currentLexeme.updateType(Keyword);
					addLexeme(currentLexeme);
				}
				if (currentLexeme.isIdentifier()) { addLexeme(currentLexeme); }
				if (currentLexeme.isReal()) { 
					std::cout << "tryna add real: " << currentLexeme.getToken() << std::endl;
					addLexeme(currentLexeme); 
				}

				//if (currentLexeme.isInteger())		{ addLexeme(currentLexeme); }
				if (isSeparator(input)) { addLexeme(Token(1, true, input, Separator)); }

				if (isOperator(input)) {
					if (isOperator(currentLexeme.getToken() + input)) {
						currentLexeme.updateToken(input);
						currentLexeme.updateType(Operator);
						addLexeme(currentLexeme);
					}
					else {
						addLexeme(currentLexeme);
					}
				}

				currentLexeme.clear();
			}
			// Don't want to make currentLexeme = " "
			//if (!isSpace(input)) {
			//	currentLexeme.clear();
			//	currentLexeme.updateToken(input);
			//}
			else {
				// Get the next input in inputCode
				currentLexeme.updateToken(input);
				if (!(charToTokenType(input) == Integer && currentLexeme.getType() == Real)) {
					currentLexeme.updateType(charToTokenType(input));
				}
			}
			/*
			// we are reading an integer
			if (isDigit(input)) {

			}

			// We are reading an identifier or keyword
			if (isAlphaOrPound(input)) {

			}

			 //if input's token possibilities match c's (i.e input is a letter and c has letters
			 //if input is an inter and c is an identifier/keyword, then we break

			currentLexeme.updateToken(input);
			currentLexeme.updateState(input);


			if (isDigit(c) && !isReal) {
				// state = Integer.nextState()
			}
			// Check if c builds a real
			if (isDigit(c) && isReal) {
				// runStackThroughReals()
			}
			if (c == ".") {
				isReal = true;
			}
			*/
		} /* end for loop */
	} /* end function */


	void printLexemes() {
		std::cout << "\nPrinting lexemes\n";
		for (std::list<Pair>::iterator it = lexemes_.begin(); it != lexemes_.end(); it++) {
			std::cout << *it << std::endl;
		}
	}

private:
	std::list<Pair> lexemes_;
};


int main() {

	std::string inputCode = "while (fahr#x < upper) a = 23.00 ";
	Token token;
	Lexer lexer;
	lexer.getTokens(inputCode);
	lexer.printLexemes();

	return 0;
}