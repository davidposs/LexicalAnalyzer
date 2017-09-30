CPSC 323 - Compilers and Languages

Project 1: Lexical Analyzer

Members: David Poss    : davidposs@csu.fullerton.edu
	 Douglas Galm  : douglasgalm@csu.fullerton.edu

Usage:   Reads from "sample.txt" input file (must be in same directory as code)
	 and creates an output file "finished.txt", also in the same directory.

Notes:   Output includes lexemes found and correctly categorized by the lexer,
         and also includes tokens it failed to match correctly, such as characters
         like !, $, &, et cetera.	 


Compiled in Visual Studio 2017

Linux Instructions (tested on Ubuntu 16.04) 
1. To compile:		$ g++ -o lexer *.cpp *.h -std=c++11
2. To run executable:	$./lexer