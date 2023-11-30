/* TestParserMain.cpp */
#include <iostream>
#include <string>
#include "../include/Lexer.h"
#include "../include/Parser.h"

using namespace std;

int usage(const char *const procname)
{
	cout << "Usage: " << procname << " Ҫ����C�����ļ�" << endl
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
	// �����Ŀ��ӻ���Ϊ�����֣��﷨����ջ�ı仯������main.pyʵ��
}