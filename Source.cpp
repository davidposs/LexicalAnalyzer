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
#define DEFAULT_STATE 0

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
	/* State 1 */	1,	-1,
	/* State 2 */	2,	 2,
	/* State 3 */	3,	-1,
	/* State 4 */	3,	-1
	/* -1 = empty set			*/
};
const int realAcceptStates[] = { 3 };

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
	return isdigit(str[0]);
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
		type_ = Unknown;
		state_ = 0;
		accept_ = false;
	}

	void updateState(std::string input) {
		if (type_ == Integer && isDigit(input)) {
			std::cout << "input is: " << input << std::endl;
			state_ = intAcceptStates[0];
			accept_ = true;
		}

		if (type_ == Real && (isDigit(input)) || input == ".") {
			if (state_ >= 0) {
				if (input == ".") {
					std::cout << "found a period!  " << state_ << std::endl << std::endl;
					state_ = realTable[state_][1];
				}
				else {
					state_ = realTable[state_][0];
				}
				accept_ = (state_ == realAcceptStates[0]);
			}
		}
		else {
			accept_ = false;
		}
	
		


		/* Identifier */
		if (type_ == Identifier && isAlphaOrPound(input)) {
			if (state_ >= 0) {
				if (input == "#") {
					state_ = idTable[state_][1];
				}
				else if (isalpha(input[0])) {
					state_ = idTable[state_][0];
				}
				accept_ = (state_ == idAcceptStates[0] || state_ == idAcceptStates[1]);
			}
			else {
				accept_ = false;
			}
		}
	} /* end updateState */

	void updateState(int state) {
		state_ = state;
	}

	friend std::ostream& operator<<(std::ostream& os, const Token& t) {
		os << t.token_ << "\t\t" << t.type_ << std::endl;
		return os;
	}

	bool isAccepted() { return accept_; }
	TokenType getType() { return type_; }
	std::string getToken() { return token_; }
	int getState() { return state_; }

private:
	int			state_;
	bool		accept_;
	TokenType	type_;
	std::string token_;
};

struct Pair {
	Pair() : val1("empty"), val2("empty") {}
	Pair(std::string l, std::string t) : val1(l), val2(t) {}

	friend std::ostream& operator<<(std::ostream& os, Pair P) {
		os << "Token: " << P.val1 << "\t\t Type: " << P.val2;
		return os;
	}

	std::string val1, val2;
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
	void addLexeme(Token& lexeme) {
		if (!lexeme.isAccepted()) {
			std::cout << "Token " << lexeme.getToken() << " not accepted " << std::endl;
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
			lexemes_.push_back(Pair(lexeme.getToken(), "separator"));
			break;
		default:
			return;
		}
		lexeme.clear();
		return;
	}

	/*		Main lexer function		*/
	void getTokens(std::string inputCode) {
		/* Read character from file one at a time, until eof
		std::fstream inputFile;
		inputFile.open(filename, 'r');
		while (inputFile.get(c)) { */

		Token currentLexeme;

		for (std::string::iterator i = inputCode.begin(); i != inputCode.end(); i++) {
			std::string input = std::string(1, *i);

			if (charToTokenType(input) == Real && currentLexeme.getType() == Integer) {
				currentLexeme.updateState(realTable[2][0]);
				currentLexeme.updateType(Real);
			}
			else if (charToTokenType(input) == Integer && currentLexeme.getType() != Real) {
				currentLexeme.updateType(Integer);
				std::cout << currentLexeme.getToken() << std::endl;
			}

			// See if input terminates a token
			if (isSeparator(input) || isOperator(input) || isSpace(input)) {
				if (isKeyword(currentLexeme.getToken())) {
					currentLexeme.updateType(Keyword);
					addLexeme(currentLexeme);
				}
				if (isSpace(input) && currentLexeme.getToken() != "") {
					addLexeme(currentLexeme);
				}
				if (currentLexeme.isIdentifier()) {
					addLexeme(currentLexeme);
				}
				if (currentLexeme.isInteger()) { 
					std::cout << currentLexeme.getToken() << " should be an integer\n";
					addLexeme(currentLexeme); 
				}
				if (currentLexeme.isReal()) {
					addLexeme(currentLexeme);
				}
				if (isSeparator(input)) {
					addLexeme(Token(0, true, input, Separator));
				}
				if (isOperator(input)) {
					/* See if next token is also an operator, like <= */
					if (isOperator(input + *(i + 1))) {
						addLexeme(Token(0, true, input + *(i + 1), Operator));
						i++;
					}
					else { addLexeme(Token(0, true, input, Operator)); }
				}

				currentLexeme.clear();
			}
			// Don't want to make currentLexeme = " "
			//if (!isSpace(input)) {
			//	currentLexeme.clear();
			//	currentLexeme.updateToken(input);
			//}
			else { /* input doesn't terminate a token */
				if (isDigit(input) && currentLexeme.getType() == Integer) {
					currentLexeme.updateToken(input);
					std::cout << "\ninteger: " << currentLexeme.getToken() << std::endl;
					currentLexeme.updateState(input);
						std::cout << "11 should be lkjhlkjaccepted\n\n";
					if (currentLexeme.isAccepted()) {
					}
				}
				else if (input == "." || (isDigit(input) && currentLexeme.getType() == Real)) {
					currentLexeme.updateToken(input);
					currentLexeme.updateState(input);
				}
				// We are reading an identifier or keyword
				else if (isAlphaOrPound(input)) {
					currentLexeme.updateToken(input);
					currentLexeme.updateType(Identifier);
					currentLexeme.updateState(input);
				}
			}
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

	std::string inputCode = "while (fahr#x <= upper) a = 23.00  f##f #ff f#f#ff 11 43.34.32 ";
	Token token;
	Lexer lexer;
	lexer.getTokens(inputCode);
	lexer.printLexemes();

	return 0;
}