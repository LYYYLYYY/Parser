/* Grammar2Tables.h LR1文法分析 */

#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <sstream>
#include "Error.h"

using namespace std;

/***************************************************************************
  类名：Generate
  功能：表示一个产生式
  成员：string lhs-产生式左, vector<string> rhs-产生式右
  说明：
***************************************************************************/
class Generate
{
public:
	string lhs;
	vector<string> rhs;

	Generate(const string &a, const vector<string> &b) : lhs(a), rhs(b) {}
};

bool operator<(const Generate &, const Generate &);

/***************************************************************************
  类名：LR0Project
  功能：表示一个LR0项目
  成员：nowPosition-当前停的位置
  说明：继承Generate
***************************************************************************/
class LR0Project : public Generate
{
public:
	unsigned int nowPosition;

	string GetNowString(void);
	LR0Project(const string &a, const vector<string> &b, unsigned int c) : Generate(a, b), nowPosition(c) {}
	LR0Project(const Generate &b, unsigned int c) : Generate(b.lhs, b.rhs), nowPosition(c) {}
};

/***************************************************************************
  类名：LR1Project
  功能：表示一个LR1项目
  成员：nxtstr-向后展望的一个终结符
  说明：继承LR0Project
***************************************************************************/
class LR1Project : public LR0Project
{
public:
	string nxtstr;

	LR1Project(const string &a, const vector<string> &b, unsigned int c, const string &d) : LR0Project(a, b, c), nxtstr(d) {}
	LR1Project(const Generate &b, unsigned int c, const string &d) : LR0Project(b, c), nxtstr(d) {}
};

bool operator<(const LR1Project &, const LR1Project &);
bool operator==(const LR1Project &, const LR1Project &);

/***************************************************************************
  类名：Closure
  功能：表示一个LR1项目集
  成员：shiftpro-移进项目和待约项目集合, reducepro-归约项目和接受项目集合
  说明：
***************************************************************************/
class Closure
{
public:
	set<LR1Project> shiftpro, reducepro;

	bool operator==(const Closure &);
	int size(void);
	void insert(const LR1Project &);
	void Complete(const vector<Generate> &, map<string, vector<string>>);
};

/***************************************************************************
  类名：Grammar2Tables
  功能：表示文法处理部件
  成员：
  说明：
***************************************************************************/
class Grammar2Tables
{
private:
	string GrammarFilename;
	map<string, int> GrammarTokens; // map<int,int> a; a[1]=1,a[2]=2;cout<<a[1]<<' '<<a[3];// 1 0
	vector<Generate> GrammarGenerates;
	map<Generate, unsigned int> Generate_Id;
	map<string, vector<string>> GrammarFIRST, GrammarFOLLOW;
	vector<LR0Project> GrammarLR0Projects;
	vector<Closure> GrammarClosures;
	map<pair<int, string>, int> GrammarGo; // GrammarGo[make_pair(1,"+")]=2
	map<int, map<string, pair<char, int>>> GrammarACTION, GrammarGOTO;

	void GetTokens(void);
	void GetGenerates(void);
	void GetFIRST(void);
	void GetFOLLOW(void);
	void GetLR0Projects(void);
	void GetClosuresAndGo(void);
	void GetACTIONAndGOTO(void);

public:
	Grammar2Tables(const string &, int = 0);
	bool CheckIsLR1(const string & = "CON");
	void DebugGrammarTokens(const string & = "CON");
	void DebugGrammarGenerates(const string & = "CON");
	void DebugGrammarFIRST(const string & = "CON");
	void DebugGrammarFOLLOW(const string & = "CON");
	// void DebugGrammarLR0Projects(const string & = "CON");
	void DebugGrammarClosures(const string & = "CON");
	// void DebugGrammarGo(const string & = "CON");
	void DebugGrammarACTIONAndGOTO(const string & = "CON");
	void DebugGrammarACTIONAndGOTOprivate(const string & = "CON");
};
