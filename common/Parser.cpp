/* Parser.cpp */
#include "../include/Parser.h"

/***************************************************************************
  函数名称：GetGenerates
  功    能：将生成式通过文件方式装入Parser中
  输入参数：string filename
  返 回 值：
  说    明：
***************************************************************************/
void Parser::GetGenerates(const string &filename)
{
	Generates.clear();
	string ss;
	ifstream fin;
	fin.open(filename, ios::in);
	if (!fin.is_open())
	{
		Error(-1);
	}
	while (getline(fin, ss))
	{
		istringstream sin(ss);
		string theleft;
		sin >> theleft;
		if (sin.fail())
			break;
		vector<string> theright;
		string tmp;
		sin >> tmp;
		while (sin >> tmp)
		{
			theright.push_back(tmp);
		}
		Generates.push_back(Generate(theleft, theright));
	}
	fin.close();
}

/***************************************************************************
  函数名称：GetACTIONandGOTO
  功    能：将ACTION和GO通过文件方式装入Parser中
  输入参数：string filename
  返 回 值：
  说    明：ACTION和GO先前都放到同一个文件(ACTION和GO本质上是一个东西)中了，文件格式有约定。
***************************************************************************/
void Parser::GetACTIONandGOTO(const string &filename)
{
	ACTION.clear();
	GOTO.clear();
	string ss;
	ifstream fin;
	fin.open(filename, ios::in);
	if (!fin.is_open())
	{
		Error(-1);
	}
	while (getline(fin, ss))
	{
		istringstream sin(ss);
		int a, d;
		string b;
		char c;
		sin >> a;
		if (sin.fail())
			break;
		sin >> b >> c >> d;
		if (c == 'g')
			GOTO[a][b] = make_pair(c, d);
		else
			ACTION[a][b] = make_pair(c, d);
	}
	fin.close();
}

/***************************************************************************
  函数名称：PreProcess
  功    能：删除字符串中的注释
  输入参数：string-待处理字符串, bool &in_exegesis-处理之前字符串是否已经处于多行注释中("/ * * /"这种)的标志(要实时维护)
  返 回 值：处理之后的字符串
  说    明：一行一行处理，因为一行中可能有多个注释的位置，所以需要递归处理
***************************************************************************/
string Parser::PreProcess(const string &ss, bool &in_exegesis)
{
	int n = ss.length();
	for (int i = 0; i < n - 1; ++i)
	{
		string tmp = ss.substr(i, 2);
		if (tmp == "//" && !in_exegesis)
			return ss.substr(0, i);
		else if (tmp == "/*" && !in_exegesis)
			return ss.substr(0, i) + PreProcess(ss.substr(i + 2), in_exegesis = true);
		else if (tmp == "*/" && in_exegesis)
			return PreProcess(ss.substr(i + 2), in_exegesis = false);
	}
	return in_exegesis ? "" : ss;
}

/***************************************************************************
  函数名称：OUTPUT
  功    能：根据语法分析栈中的内容进行形式化输出
  输入参数：STK-语法分析栈, followendl-保存单词之后是否跟有换行的栈
  返 回 值：
  说    明：只有当打开开关的时候会进行这个输出。每行的输出之间需要按任意键继续。
***************************************************************************/
void Parser::OUTPUT(const stack<pair<int, string>> &STK, const stack<bool> &followendl)
{
	stack<pair<int, string>> STK1 = STK, STK2;
	stack<bool> followendl1 = followendl, followendl2;
	while (!STK1.empty())
	{
		STK2.push(STK1.top()), STK1.pop();
		followendl2.push(followendl1.top()), followendl1.pop();
	}
	cout << endl
		 << "-----------------------------------stack" << endl;
	while (!STK2.empty())
	{
		cout << STK2.top().second;
		cout << (followendl2.top() ? '\n' : ' ');
		STK2.pop(), followendl2.pop();
	}
	cout << endl
		 << "-----------------------------------stack" << endl;
	getchar();
}

/***************************************************************************
  函数名称：stringinfile
  功    能：返回一个字符串的转义表达形式
  输入参数：例 "a"
  返 回 值：例 \"a\" (要输出这个字符串又要转义为 \\\"a\\\" )
  说    明：只判断首尾字符是不是双引号的情况
***************************************************************************/
string Parser::stringinfile(const string &ss)
{
	return ss[0] == '\"' ? "\\\"" + ss.substr(1, ss.length() - 2) + "\\\"" : ss;
}

/***************************************************************************
  函数名称：Parser
  功    能：根据产生式文件和ACTIONAndGOTO文件初始化产生式组、ACTION表和GOTO表
  输入参数：s1-产生式文件名, s2-ACTIONAndGOTO文件名
  返 回 值：
  说    明：
***************************************************************************/
Parser::Parser(const string &s1, const string &s2)
{
	GetGenerates(s1);
	GetACTIONandGOTO(s2);
}

/***************************************************************************
  函数名称：AnalyzeAndOutput
  功    能：对程序源文件进行语法分析，并提供过程输出选项
  输入参数：filename-源文件名, op-是否进行过程的输出
  返 回 值：无输出值，但会生成输出文件：nodes.txt-后续语法树可视化输入文件, actions.json-语法分析栈的动作序列
  说    明：语法分析过程中调用词法分析，以一行为单位进行处理，源程序只扫描一遍即可完成语法分析
***************************************************************************/
void Parser::AnalyzeAndOutput(const string &filename, int op)
{
	TokenClear();
	string ss;
	ifstream fin;
	ofstream fout1, fout2;
	stack<pair<int, string>> STK;
	stack<bool> followendl;
	stack<int> node_id;
	vector<Token> newtokens;
	vector<pair<int, string>> tmpnode;
	bool in_exegesis = false;
	int tot = -1, nowline = 0;

	fin.open(filename, ios::in);
	if (!fin.is_open())
	{
		Error(-1);
	}
	fout1.open("../files/nodes.txt", ios::out);
	if (!fout1.is_open())
	{
		Error(-1);
	}
	fout2.open("../files/actions.json", ios::out);
	if (!fout2.is_open())
	{
		Error(-1);
	}
	// 初始把"#"压栈
	STK.push(make_pair(0, "#"));
	node_id.push(tot);
	followendl.push(false);

	fout2 << '[' << endl;
	fout2 << "{ \"Stack\": { \"action\": \"push\", \"element\": [\"" << stringinfile(STK.top().second) << "\", \"" << STK.top().first << "\"] }, ";

	while (true)
	{
		nowline++;
		bool flg = true;
		if (!getline(fin, ss))
			flg = false;
		// 输入到最后就人工生成一个只有NONE的token序列，表示最后的"#"
		if (flg)
			newtokens = ScanNewLine(PreProcess(ss, in_exegesis), nowline);
		else
			newtokens.clear(), newtokens.push_back(Token(TokenCode::NONE));
		for (unsigned int i = 0; i < newtokens.size(); ++i)
		{
			if (op)
				OUTPUT(STK, followendl);
			string a = newtokens[i].Type2String();
			if (a == "Endl")
			{
				followendl.pop();
				followendl.push(true);
				continue;
			}

			if (ACTION[STK.top().first].count(a))
			{
				// 接受项目
				if (ACTION[STK.top().first][a].first == 'a')
					break;
				// 移进项目
				if (ACTION[STK.top().first][a].first == 's')
				{
					STK.push(make_pair(ACTION[STK.top().first][a].second, a));
					node_id.push(++tot);
					followendl.push(false);

					fout2 << "\"Token\": \"" << stringinfile(a) << "\", \"Action\": \""
						  << "移进 s" << ACTION[STK.top().first][a].second << "\" }," << endl;
					fout2 << "{ \"Stack\": { \"action\": \"push\", \"element\": [\"" << stringinfile(STK.top().second) << "\", \"" << STK.top().first << "\"] }, ";
				}
				// 归约项目
				else
				{
					tmpnode.clear();
					Generate tmp = Generates[ACTION[STK.top().first][a].second];
					for (unsigned int j = tmp.rhs.size() - 1; j >= 0; --j)
					{
						if (STK.empty() || STK.top().second != tmp.rhs[j])
							return Error(3, nowline);

						tmpnode.push_back(make_pair(node_id.top(), STK.top().second));

						STK.pop();
						node_id.pop();
						followendl.pop();

						fout2 << "\"Token\": \"" << stringinfile(a) << "\", \"Action\": \""
							  << "归约 r" << ACTION[STK.top().first][a].second << "\" }," << endl;
						fout2 << "{ \"Stack\": { \"action\": \"pop\" }, ";
						if (j == 0)
							break; // unsigned int
					}
					// 待约项目(的存在性判断)
					if (STK.empty() || !GOTO[STK.top().first].count(tmp.lhs))
						return Error(4, nowline);

					STK.push(make_pair(GOTO[STK.top().first][tmp.lhs].second, tmp.lhs));
					node_id.push(++tot);
					followendl.push(false);

					fout2 << "\"Token\": \"" << stringinfile(tmp.lhs) << "\", \"Action\": \""
						  << "GOTO " << ACTION[STK.top().first][a].second << "\" }," << endl;
					fout2 << "{ \"Stack\": { \"action\": \"push\", \"element\": [\"" << stringinfile(STK.top().second) << "\", \"" << STK.top().first << "\"] }, ";
					for (auto xx : tmpnode)
						fout1 << xx.first << ' ' << xx.second << ' ' << tot << endl;
					--i;
				}
			}
			else
				return Error(5, nowline);
		}
		if (!flg)
			break;
	}
	fout1 << tot << ' ' << STK.top().second << ' ' << '-' << endl;
	fout2 << "\"Token\": \"" << stringinfile("#") << "\", \"Action\": \""
		  << "acc"
		  << "\" }," << endl;
	fout2 << "{ \"Stack\": { \"action\": \"pop\" }, ";
	fout2 << "\"Token\": \"" << stringinfile("#") << "\", \"Action\": \""
		  << "STOP"
		  << "\" }" << endl;
	fout2 << ']' << endl;
	fin.close();
	fout1.close();
	fout2.close();

	if (op)
		OUTPUT(STK, followendl);
	// 最后还需判断栈中内容是否清空
	if (STK.size() == 2)
		cout << "Is C !" << endl;
	else
		return Error(2);
}

/***************************************************************************
  函数名称：Debug
  功    能：Parser的调试输出
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void Parser::Debug()
{
	cout << Generates.size() << endl;
	cout << Generates[0].rhs.size() << endl;
	cout << Generates[1].rhs.size() << endl;
	cout << Generates[2].rhs.size() << endl;
	cout << ACTION.size() << endl;
	cout << GOTO.size() << endl;
}