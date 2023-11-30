/* TestParserMain.cpp */
#include <iostream>
#include <string>
#include "../include/Lexer.h"
#include "../include/Parser.h"

using namespace std;

int usage(const char *const procname)
{
	cout << "Usage: " << procname << " 要检查的C语言文件" << endl
		 << endl;
	cout << "e.g. : " << procname << " test.c" << endl;

	return 0;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
		return usage(argv[0]), 0;
	Parser B("../files/Generates.txt", "../files/ACTIONAndGOTOPrivate.txt");
	B.AnalyzeAndOutput(argv[1]);
	// B.AnalyzeAndOutput(argv[1],1);
	return 0;
	// 后续的可视化分为两部分：语法树和栈的变化，均由main.py实现
}