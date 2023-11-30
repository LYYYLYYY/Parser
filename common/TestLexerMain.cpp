/* TestLexerMain.cpp */
#include <iostream>
#include <string>
#include "../include/Lexer.h"

using namespace std;

int main()
{
	Lexer A("../files/test.cpp");
	A.Debug();
	return 0;
}