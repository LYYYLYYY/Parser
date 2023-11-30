/* Lexer.h 词法分析器，被语法分析器调用 */

#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <fstream>
#include "Token.h"
#include "Error.h"

using namespace std;

/***************************************************************************
  类名：IdMessage
  功能：表示符号表中的每项
  成员：待补充
  说明：
***************************************************************************/
struct IdMessage
{
	string name;
};

/***************************************************************************
  类名：ConstMessage
  功能：表示常数表中的每项
  成员：待补充
  说明：
***************************************************************************/
struct ConstMessage
{
	string value;
};

/***************************************************************************
  类名：Lexer
  功能：词法分析器
  成员：tokens-存储程序转化成的token序列
  说明：正常情况下不会用到tokens，因为程序一遍就要完成词法分析+语法分析
***************************************************************************/
class Lexer
{
private:
	vector<Token> tokens;
	// vector<IdMessage> IdMap;
	// vector<ConstMessage> ConstMap;
	
	bool IsLetter(const char);
	bool IsBlank(const char);
	char Getchar(const string &, int);
	bool GetBC(const string &, int &);
	int InsertId(const string &);
	int InsertConst(const string &);
	pair<TokenCode, int> Reserve(const string &);

public:
	Lexer(){};
	Lexer(const string &);
	void TokenClear(void);
	vector<Token> GiveTokens(void);
	vector<Token> ScanNewLine(const string &, int);
	void Debug(void);
};