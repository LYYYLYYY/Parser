/* Parser.h 语法分析器，继承(调用)词法分析器 */

#pragma once

#include <map>
#include <string>
#include <stack>
#include "Token.h"
#include "Grammar2Tables.h"
#include "Error.h"
#include "Lexer.h"

using namespace std;

/***************************************************************************
  类名：Parser
  功能：语法分析器
  成员：Generates-生成式组, ACTION-ACTION表, GOTO-GOTO表
  说明：继承Lexer
***************************************************************************/
class Parser : public Lexer
{
private:
	vector<Generate> Generates;
	map<int, map<string, pair<char, int>>> ACTION, GOTO;

	void GetGenerates(const string &);
	void GetACTIONandGOTO(const string &);
	string PreProcess(const string &, bool &);
	void OUTPUT(const stack<pair<int, string>> &, const stack<bool> &);
	string stringinfile(const string&);

public:
	Parser(const string &, const string &);
	void AnalyzeAndOutput(const string &, int = 0);
	void Debug(void);
};