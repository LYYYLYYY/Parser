/* Grammar2Tables.cpp */
#include "../include/Grammar2Tables.h"

/***************************************************************************
  函数名称：operator<
  功    能：重载Generate的<
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
bool operator<(const Generate &x, const Generate &y)
{
	if (x.lhs != y.lhs)
		return x.lhs < y.lhs;
	return x.rhs < y.rhs;
}

/***************************************************************************
  函数名称：GetNowString
  功    能：获取LR0项目当前位置的符号
  输入参数：
  返 回 值：string
  说    明：
***************************************************************************/
string LR0Project::GetNowString()
{
	return nowPosition == rhs.size() ? "" : rhs[nowPosition];
}

/***************************************************************************
  函数名称：operator<
  功    能：重载LR1Project的<
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
bool operator<(const LR1Project &x, const LR1Project &y)
{
	if (x.lhs != y.lhs)
		return x.lhs < y.lhs;
	if (x.rhs != y.rhs)
		return x.rhs < y.rhs;
	if (x.nowPosition != y.nowPosition)
		return x.nowPosition < y.nowPosition;
	return x.nxtstr < y.nxtstr;
}

/***************************************************************************
  函数名称：operator==
  功    能：重载LR1Project的==
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
bool operator==(const LR1Project &x, const LR1Project &y)
{
	return x.lhs == y.lhs && x.rhs == y.rhs && x.nowPosition == y.nowPosition && x.nxtstr == y.nxtstr;
}

/***************************************************************************
  函数名称：operator==
  功    能：重载Closure的==
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
bool Closure::operator==(const Closure &x)
{
	return shiftpro == x.shiftpro && reducepro == x.reducepro;
}

/***************************************************************************
  函数名称：size
  功    能：获得Closure的总大小
  输入参数：
  返 回 值：int
  说    明：Closure有两个部分，大小需要相加
***************************************************************************/
int Closure::size()
{
	return shiftpro.size() + reducepro.size();
}

/***************************************************************************
  函数名称：insert
  功    能：讲一个LR1项目插入到Closure中
  输入参数：LR1Project
  返 回 值：
  说    明：要判断插入到Closure的哪个部分
***************************************************************************/
void Closure::insert(const LR1Project &x)
{
	if (x.nowPosition == x.rhs.size())
		reducepro.insert(x);
	else
		shiftpro.insert(x);
}

/***************************************************************************
  函数名称：Complete
  功    能：完善Closure，把符合条件的LR1项目都加入到Closure中
  输入参数：GrammarGenerates, GrammarFIRST
  返 回 值：
  说    明：此为复杂度瓶颈所在
***************************************************************************/
void Closure::Complete(const vector<Generate> &GrammarGenerates, map<string, vector<string>> GrammarFIRST)
{
	map<string, vector<vector<string>>> Generates0;
	for (auto x : GrammarGenerates)
	{
		Generates0[x.lhs].push_back(x.rhs);
	}
	while (true)
	{
		Closure oldans = *this;
		for (auto x : oldans.shiftpro)
		{
			string y = x.GetNowString();
			if (x.nowPosition == x.rhs.size() - 1)
			{
				for (auto z : Generates0[y])
					this->shiftpro.insert(LR1Project(y, z, 0, x.nxtstr));
			}
			else
			{
				for (auto z : Generates0[y])
					for (auto w : GrammarFIRST[x.rhs[x.nowPosition + 1]])
						this->shiftpro.insert(LR1Project(y, z, 0, w));
			}
		}
		if (this->size() == oldans.size())
			break;
	}
}

/***************************************************************************
  函数名称：GetTokens
  功    能：获得文法中的Token，并对Token进行分类(终结符0，非终结符1)
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void Grammar2Tables::GetTokens()
{
	ifstream fin;
	fin.open(GrammarFilename, ios::in);
	if (!fin.is_open())
	{
		Error(-1);
	}
	string ss, lst;
	fin >> lst;
	GrammarTokens.clear();
	while (fin >> ss)
	{
		// cout<<ss<<endl;
		if (ss == "->")
			GrammarTokens[lst] = 1;
		else if (ss == "|")
			GrammarTokens[lst] |= 0;
		else if (lst != "->" && lst != "|")
			GrammarTokens[lst] |= 0;
		lst = ss;
	}
	GrammarTokens[lst] |= 0;
	fin.close();
}

/***************************************************************************
  函数名称：GetGenerates
  功    能：获得文法的产生式组
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void Grammar2Tables::GetGenerates()
{
	GrammarGenerates.clear();
	ifstream fin;
	fin.open(GrammarFilename, ios::in);
	if (!fin.is_open())
	{
		Error(-1);
	}
	string ss;
	while (getline(fin, ss))
	{
		istringstream sin(ss);
		string tmp1;
		vector<string> tmp2;
		sin >> tmp1;
		string tmp3;
		sin >> tmp3;
		while (sin >> tmp3)
		{
			if (tmp3 == "|")
				GrammarGenerates.push_back(Generate(tmp1, tmp2)), tmp2.clear();
			else
				tmp2.push_back(tmp3);
		}
		if (tmp2.size())
			GrammarGenerates.push_back(Generate(tmp1, tmp2));
	}
	for (unsigned int i = 0; i < GrammarGenerates.size(); ++i)
	{
		Generate_Id[GrammarGenerates[i]] = i;
	}
	fin.close();
}

/***************************************************************************
  函数名称：GetFIRST
  功    能：获得文法的FIRST
  输入参数：
  返 回 值：
  说    明：没有处理产生式有ε的情况，需要保证文法中没有ε
***************************************************************************/
void Grammar2Tables::GetFIRST()
{
	GrammarFIRST.clear();
	for (auto x : GrammarTokens)
	{
		if (x.second == 0)
			GrammarFIRST[x.first].push_back(x.first);
	}
	while (true)
	{
		bool flg = false;
		for (auto x : GrammarGenerates)
		{
			for (auto y : GrammarFIRST[x.rhs[0]])
			{
				bool had = false;
				for (auto z : GrammarFIRST[x.lhs])
				{
					if (z == y)
					{
						had = true;
						break;
					}
				}
				if (!had)
				{
					GrammarFIRST[x.lhs].push_back(y);
					flg = true;
				}
			}
		}
		if (!flg)
			break;
	}
}

/***************************************************************************
  函数名称：GetFOLLOW
  功    能：获得文法的FOLLOW
  输入参数：
  返 回 值：
  说    明：没有处理产生式有ε的情况，需要保证文法中没有ε
***************************************************************************/
void Grammar2Tables::GetFOLLOW()
{
	GrammarFOLLOW.clear();
	GrammarFOLLOW[GrammarGenerates[0].lhs].push_back("#");
	while (true)
	{
		bool flg = false;
		for (auto x : GrammarGenerates)
		{
			for (unsigned int i = 0; i < x.rhs.size(); ++i)
			{
				string y = x.rhs[i];
				if (i == x.rhs.size() - 1)
				{
					for (auto z1 : GrammarFOLLOW[x.lhs])
					{
						bool had = 0;
						for (auto z2 : GrammarFOLLOW[y])
						{
							if (z2 == z1)
							{
								had = 1;
								break;
							}
						}
						if (!had)
						{
							GrammarFOLLOW[y].push_back(z1);
							flg = true;
						}
					}
				}
				else
				{
					string z = x.rhs[i + 1];
					for (auto z1 : GrammarFIRST[z])
					{
						bool had = 0;
						for (auto z2 : GrammarFOLLOW[y])
						{
							if (z2 == z1)
							{
								had = 1;
								break;
							}
						}
						if (!had)
						{
							GrammarFOLLOW[y].push_back(z1);
							flg = true;
						}
					}
				}
			}
		}
		if (!flg)
			break;
	}
}

/***************************************************************************
  函数名称：GetLR0Projects
  功    能：获得文法的所有LR0项目
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void Grammar2Tables::GetLR0Projects()
{
	GrammarLR0Projects.clear();
	for (auto x : GrammarGenerates)
	{
		for (unsigned int i = 0; i <= x.rhs.size(); ++i)
			GrammarLR0Projects.push_back(LR0Project(x, i));
	}
}

/***************************************************************************
  函数名称：GetClosuresAndGo
  功    能：获得文法的所有LR1项目的闭包(Closures)和闭包之间的转移(Go)
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void Grammar2Tables::GetClosuresAndGo()
{
	GrammarClosures.clear();
	GrammarGo.clear();
	Closure nwclosure;
	nwclosure.insert(LR1Project(GrammarGenerates[0], 0, "#"));
	nwclosure.Complete(GrammarGenerates, GrammarFIRST);
	GrammarClosures.push_back(nwclosure);
	unsigned int op = 0; // operate-position
	while (op < GrammarClosures.size()) // 类似广度优先搜索的思路，不断扩展直到所有闭包的Go都求出来为止
	{
		map<string, Closure> string2closure;
		for (auto x : GrammarClosures[op].shiftpro)
			string2closure[x.GetNowString()].insert(LR1Project(x.lhs, x.rhs, x.nowPosition + 1, x.nxtstr));

		for (auto j = string2closure.begin(); j != string2closure.end(); ++j)
		{
			nwclosure = j->second;
			nwclosure.Complete(GrammarGenerates, GrammarFIRST);
			bool flg = false;
			for (unsigned int i = 0; i < GrammarClosures.size(); ++i)
				if (nwclosure == GrammarClosures[i])
				{
					GrammarGo[make_pair(op, j->first)] = i;
					flg = true;
					break;
				}
			if (!flg)
				GrammarClosures.push_back(nwclosure), GrammarGo[make_pair(op, j->first)] = GrammarClosures.size() - 1;
		}
		++op;
	}
}

/***************************************************************************
  函数名称：GetACTIONAndGOTO
  功    能：使用LR1处理方法，获得文法的ACTION表和GOTO表
  输入参数：
  返 回 值：
  说    明：文法严格意义上不是LR1文法，存在if-else的冲突，但是可以根据C语言规则手动消除，即优先移进(程序表中需要先填写r/a，后填写s/g)
***************************************************************************/
void Grammar2Tables::GetACTIONAndGOTO()
{
	for (unsigned int i = 0; i < GrammarClosures.size(); ++i)
	{
		for (auto x : GrammarClosures[i].reducepro)
		{
			GrammarACTION[i][x.nxtstr] = make_pair('r', Generate_Id[Generate(x.lhs, x.rhs)]);
			if (Generate_Id[Generate(x.lhs, x.rhs)] == 0)
			{
				GrammarACTION[i][x.nxtstr].first = 'a';
			}
		}
	}
	// 先ACTION后GOTO，目的是移进优先
	for (auto x : GrammarGo)
	{
		if (GrammarTokens[x.first.second])
			GrammarGOTO[x.first.first][x.first.second] = make_pair('g', x.second);
		else
			GrammarACTION[x.first.first][x.first.second] = make_pair('s', x.second);
	}
}

/***************************************************************************
  函数名称：Grammar2Tables
  功    能：初始化文法处理模块
  输入参数：filename-文法所在文件名, op-是否在文件相同情况下不初始化
  返 回 值：
  说    明：
***************************************************************************/
Grammar2Tables::Grammar2Tables(const string &filename, int op)
{
	if (op && GrammarFilename == filename)
		return;
	GrammarFilename = filename;
	GetTokens();
	GetGenerates();
	GetFIRST();
	GetFOLLOW();
	GetLR0Projects();
	GetClosuresAndGo();
	GetACTIONAndGOTO();
}

/***************************************************************************
  函数名称：CheckIsLR1
  功    能：判断文法是否是LR1文法，并将产生的冲突输出到文件中
  输入参数：filename
  返 回 值：
  说    明：
***************************************************************************/
bool Grammar2Tables::CheckIsLR1(const string &filename)
{
	bool FLG = true;
	ofstream fout;
	fout.open(filename, ios::out);
	if (!fout.is_open())
	{
		Error(-1);
	}
	for (auto x : GrammarClosures)
	{
		set<string> stringset;
		for (auto y : x.shiftpro)
		{
			if (GrammarTokens[y.GetNowString()] == 0)
			{
				stringset.insert(y.GetNowString());
			}
		}
		bool flg = true;
		set<string> samestring;
		for (auto y : x.reducepro)
		{
			if (stringset.count(y.nxtstr))
			{
				flg = false;
				samestring.insert(y.nxtstr);
			}
			else
				stringset.insert(y.nxtstr);
		}
		if (!flg)
		{
			FLG = false;
			fout << "this is bad!" << endl;
			for (auto y : x.shiftpro)
			{
				if (!samestring.count(y.GetNowString()))
					continue;
				fout << y.lhs << " -> ";
				for (unsigned int i = 0; i < y.rhs.size(); ++i)
				{
					if (i == y.nowPosition)
						fout << ". ";
					fout << y.rhs[i] << ' ';
				}
				fout << endl
					 << y.nowPosition << ' ' << y.nxtstr << endl
					 << endl;
			}
			for (auto y : x.reducepro)
			{
				if (!samestring.count(y.nxtstr))
					continue;
				fout << y.lhs << " -> ";
				for (unsigned int i = 0; i < y.rhs.size(); ++i)
					fout << y.rhs[i] << ' ';
				fout << ". ";
				fout << endl
					 << y.nowPosition << ' ' << y.nxtstr << endl
					 << endl;
			}
		}
	}
	fout.close();
	return FLG;
}


/***************************************************************************
  函数名称：Debug...
  功    能：文法调试输出
  输入参数：filename-输出的文件名
  返 回 值：
  说    明：如果输出到控制台则filename="CON"(缺省)
***************************************************************************/

void Grammar2Tables::DebugGrammarTokens(const string &filename)
{
	cout << "----------------DebugGrammarTokens" << endl;
	freopen(filename.c_str(), "w", stdout);
	for (auto x : GrammarTokens)
	{
		cout << x.first << " ----- " << x.second << endl;
	}
	freopen("CON", "w", stdout); // 定向输出到控制台
	cout << "----------------DebugGrammarTokens" << endl;
}
void Grammar2Tables::DebugGrammarGenerates(const string &filename)
{
	cout << "----------------DebugGrammarGenerates" << endl;
	freopen(filename.c_str(), "w", stdout);
	for (auto x : GrammarGenerates)
	{
		cout << x.lhs << " -> ";
		for (auto y : x.rhs)
			cout << y << ' ';
		cout << endl;
	}
	freopen("CON", "w", stdout); // 定向输出到控制台
	cout << "----------------DebugGrammarGenerates" << endl;
}
void Grammar2Tables::DebugGrammarFIRST(const string &filename)
{
	cout << "----------------DebugGrammarFIRST" << endl;
	freopen(filename.c_str(), "w", stdout);
	for (auto x : GrammarFIRST)
	{
		cout << x.first << " (FIRST) : ";
		for (auto y : x.second)
			cout << y << ' ';
		cout << endl;
	}
	freopen("CON", "w", stdout); // 定向输出到控制台
	cout << "----------------DebugGrammarFIRST" << endl;
}
void Grammar2Tables::DebugGrammarFOLLOW(const string &filename)
{
	cout << "----------------DebugGrammarFOLLOW" << endl;
	freopen(filename.c_str(), "w", stdout);
	for (auto x : GrammarFOLLOW)
	{
		cout << x.first << " (FOLLOW) : ";
		for (auto y : x.second)
			cout << y << ' ';
		cout << endl;
	}
	freopen("CON", "w", stdout); // 定向输出到控制台
	cout << "----------------DebugGrammarFOLLOW" << endl;
}
void Grammar2Tables::DebugGrammarClosures(const string &filename)
{
	cout << "----------------DebugGrammarClosures" << endl;
	freopen(filename.c_str(), "w", stdout);
	cout << GrammarClosures.size() << endl;
	if (true)
	{
		int op = -1;
		for (auto xx : GrammarClosures)
		{
			++op;
			for (auto x : xx.shiftpro)
			{
				cout << op << " : ";
				cout << x.lhs << " -> ";
				for (unsigned i = 0; i < x.rhs.size(); ++i)
				{
					if (i == x.nowPosition)
						cout << ". ";
					cout << x.rhs[i] << ' ';
				}
				cout << ", " << x.nxtstr << endl;
			}
			for (auto x : xx.reducepro)
			{
				cout << op << " : ";
				cout << x.lhs << " -> ";
				for (auto y : x.rhs)
					cout << y << ' ';
				cout << ". ";
				cout << ", " << x.nxtstr << endl;
			}
		}
	}
	freopen("CON", "w", stdout); // 定向输出到控制台
	cout << "----------------DebugGrammarClosures" << endl;
}
void Grammar2Tables::DebugGrammarACTIONAndGOTO(const string &filename)
{
	cout << "----------------DebugGrammarACTIONAndGOTO" << endl;
	freopen(filename.c_str(), "w", stdout);
	cout << "ACTION,GOTO" << endl;
	cout << "ClosureId";
	for (auto x : GrammarTokens)
		if (!x.second)
			cout << ',' << (x.first == "," ? "\",\"" : x.first); //','处理
	cout << ',' << "#";
	for (auto x : GrammarTokens)
		if (x.second)
			cout << ',' << x.first;
	cout << endl;
	for (unsigned int i = 0; i < GrammarClosures.size(); ++i)
	{
		cout << i;
		for (auto x : GrammarTokens)
			if (!x.second)
			{
				cout << ',';
				if (GrammarACTION[i].count(x.first))
				{
					if (GrammarACTION[i][x.first].first == 'a')
						cout << "acc";
					else
						cout << GrammarACTION[i][x.first].first << GrammarACTION[i][x.first].second;
				}
			}
		cout << ',';
		if (GrammarACTION[i].count("#"))
		{
			if (GrammarACTION[i]["#"].first == 'a')
				cout << "acc";
			else
				cout << GrammarACTION[i]["#"].first << GrammarACTION[i]["#"].second;
		}
		for (auto x : GrammarTokens)
			if (x.second)
			{
				cout << ',';
				if (GrammarGOTO[i].count(x.first))
				{
					cout << GrammarGOTO[i][x.first].second;
				}
			}
		cout << endl;
	}
	freopen("CON", "w", stdout); // 定向输出到控制台
	cout << "----------------DebugGrammarACTIONAndGOTO" << endl;
}
void Grammar2Tables::DebugGrammarACTIONAndGOTOprivate(const string &filename)
{
	cout << "----------------DebugGrammarACTIONAndGOTOprivate" << endl;
	freopen(filename.c_str(), "w", stdout);
	for (auto x : GrammarACTION)
	{
		for (auto y : x.second)
		{
			// int,string,char(a,s,r),int
			cout << x.first << ' ' << y.first << ' ' << y.second.first << ' ' << y.second.second << endl;
		}
	}
	for (auto x : GrammarGOTO)
	{
		for (auto y : x.second)
		{
			// int,string,char(g),int
			cout << x.first << ' ' << y.first << ' ' << y.second.first << ' ' << y.second.second << endl;
		}
	}
	freopen("CON", "w", stdout); // 定向输出到控制台
	cout << "----------------DebugGrammarACTIONAndGOTOprivate" << endl;
}
