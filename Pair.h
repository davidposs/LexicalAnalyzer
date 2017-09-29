#pragma once
struct Pair {
	Pair() : val1("empty"), val2("empty") {}
	Pair(std::string l, std::string t) : val1(l), val2(t) {}

	Pair(Token t) : val1(t.getToken()) {
		switch (t.getType()) {
		case(1): val2 = "integer";		break;
		case(2): val2 = "real";			break;
		case(3): val2 = "idenfitier";	break;
		case(4): val2 = "operator";		break;
		case(5): val2 = "keyword";		break;
		case(6): val2 = "separator";	break;
		default: val2 = "unknown";		break;
		}
	}
	std::string val1;
	std::string val2;

	friend std::ostream& operator<<(std::ostream& os, Pair P) {
		os << P.val1 << " " << P.val2 << " ";
		return os;
	}
};