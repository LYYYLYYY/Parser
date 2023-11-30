/* Lexer.cpp */
#include "../include/Lexer.h"
#include "../include/Error.h"

/***************************************************************************
  函数名称：IsLetter
  功    能：判断是否为英文字母大小写或者下划线
  输入参数：char
  返 回 值：bool
  说    明：26*2+1=53
***************************************************************************/
bool Lexer::IsLetter(const char c)
{
	return c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

/***************************************************************************
  函数名称：IsBlank
  功    能：判断是否为常见空白字符
  输入参数：char
  返 回 值：bool
  说    明：仅常见
***************************************************************************/
bool Lexer::IsBlank(const char c)
{
	return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

/***************************************************************************
  函数名称：Getchar
  功    能：获得该字符串的当前位置的字符
  输入参数：string ss, int pos
  返 回 值：char
  说    明：如果位置指向字符串最后的\0则返回'\0'
***************************************************************************/
char Lexer::Getchar(const string &ss, int pos)
{
	return (pos < (int)ss.length()) ? ss[pos] : char('\0');
}

/***************************************************************************
  函数名称：GetBC
  功    能：跳过空白字符
  输入参数：string ss, int& nw
  返 回 值：是否跳过来某些字符
  说    明：预处理过后的空白字符不只有空格
***************************************************************************/
bool Lexer::GetBC(const string &ss, int &nw)
{
	bool flg = false;
	int n = ss.length();
	while (nw < n && IsBlank(ss[nw]))
		++nw, flg = true;
	return flg;
}

/***************************************************************************
  函数名称：InsertId
  功    能：插入符号表
  输入参数：string
  返 回 值：int-在表中的位置
  说    明：
***************************************************************************/
int Lexer::InsertId(const string &ss)
{
	return 0;
}

/***************************************************************************
  函数名称：InsertConst
  功    能：插入常数表
  输入参数：string
  返 回 值：int-在表中的位置
  说    明：
***************************************************************************/
int Lexer::InsertConst(const string &ss)
{
	return 0;
}

/***************************************************************************
  函数名称：Reserve
  功    能：识别关键字
  输入参数：string
  返 回 值：pair<TokenCode, int>-关键字类型或标识符在符号表中的位置
  说    明：pair中一定只有一个有效
***************************************************************************/
pair<TokenCode, int> Lexer::Reserve(const string &ss)
{
// 宏定义调用.inl文件
#define NUM_GRAMMAR_ENUM(num, str, id) \
	if (ss == str)                     \
		return make_pair(TokenCode::id, 0);
#include "../include/TokensReserve.inl"
#undef NUM_GRAMMAR_ENUM
	else return make_pair(TokenCode::ID, InsertId(ss));
}

/***************************************************************************
  函数名称：Lexer
  功    能：构造函数
  输入参数：string filename
  返 回 值：
  说    明：根据文件内容获得所有tokens，只用于单独测试Lexer的情况
***************************************************************************/
Lexer::Lexer(const string &filename)
{
	tokens.clear();
	string ss;
	ifstream fin;
	fin.open(filename, ios::in);
	if (!fin.is_open())
	{
		Error(-1);
	}
	int nowline = 0;
	while (getline(fin, ss))
		ScanNewLine(ss, ++nowline);
	fin.close();
}

/***************************************************************************
  函数名称：TokenClear
  功    能：清空Lexer::tokens的内容
  输入参数：
  返 回 值：
  说    明：方便继承之后调用，用于清空
***************************************************************************/
void Lexer::TokenClear()
{
	tokens.clear();
}

/***************************************************************************
  函数名称：TokenClear
  功    能：清空Lexer::tokens的内容
  输入参数：
  返 回 值：
  说    明：方便继承之后调用，用于获取所有tokens
***************************************************************************/
vector<Token> Lexer::GiveTokens()
{
	return tokens;
}

/***************************************************************************
  函数名称：ScanNewLine
  功    能：扫描字符串，生成token序列并返回。同时存储过程中生成的所有Token。
  输入参数：string-预处理后不含注释的字符串
  返 回 值：vector<Token>, 该句产生的Token序列
  说    明：
***************************************************************************/
vector<Token> Lexer::ScanNewLine(const string &ss, int nowline)
{
	vector<Token> newtokens;
	int strpos = 0, n = ss.length();
	while (strpos < n)
	{
		string strToken = "";
		if (GetBC(ss, strpos))
			newtokens.push_back(Token(TokenCode::NONE)); // 加入分隔NONE，方便判断多字符运算符
		if (strpos == n)
			break;
		char nowchar = Getchar(ss, strpos);
		// 字符串常数
		if (nowchar == '\"')
		{
			nowchar = Getchar(ss, ++strpos);
			while (strpos < n && nowchar != '\"')
				strToken += nowchar, nowchar = Getchar(ss, ++strpos);
			nowchar = Getchar(ss, ++strpos);
			int value = InsertConst(strToken);
			newtokens.push_back(Token(TokenCode::CONSTSTRING, value));
		}
		// 标识符和关键字
		else if (IsLetter(nowchar))
		{
			while (IsLetter(nowchar) || isdigit(nowchar))
				strToken += nowchar, nowchar = Getchar(ss, ++strpos);
			pair<TokenCode, int> code = Reserve(strToken); // 查询保留字的同时，插入符号表
			if (code.first == TokenCode::ID)
				newtokens.push_back(Token(TokenCode::ID, code.second));
			else
				newtokens.push_back(Token(code.first));
		}
		// 常数
		else if (isdigit(nowchar))
		{
			while (isdigit(nowchar))
				strToken += nowchar, nowchar = Getchar(ss, ++strpos);
			int value = InsertConst(strToken);
			newtokens.push_back(Token(TokenCode::CONSTVALUE, value));
		}
		// Head、运算符、界符和错误输入
		else
		{
			// Head
			if (nowchar == '#')
			{
				newtokens.push_back(Token(TokenCode::HEAD));
				break;
			}
			// 宏定义调用.inl文件, 运算符、界符和错误输入
			switch (nowchar)
			{
#define NUM_GRAMMAR_ENUM(num, str, id)             \
	case str[0]:                                   \
		newtokens.push_back(Token(TokenCode::id)); \
		break;
#include "../include/TokensSingleOperator.inl"
#undef NUM_GRAMMAR_ENUM
			default:
				Error(1, nowline);
				break;
			}
			strToken += nowchar, nowchar = Getchar(ss, ++strpos);
		}
	}
	// 多字符运算符的合成与结果生成
	vector<Token> ans;
	for (unsigned int i = 0; i < newtokens.size(); ++i)
	{
		if (i == newtokens.size() - 1)
		{
			if (newtokens[i].GetType() != TokenCode::NONE)
				tokens.push_back(newtokens[i]), ans.push_back(newtokens[i]);
		}
		else
		{
			// 两个符号之间如果有分隔则不能合成
			if (newtokens[i].GetType() == TokenCode::NONE)
				continue;
			string tmp = "";
			tmp += newtokens[i].Type2String()[1];
			tmp += newtokens[i + 1].Type2String()[1];
			if (i + 2 < newtokens.size())
				tmp += newtokens[i + 2].Type2String()[1];
			int n;

// 宏定义调用.inl文件, 判断是否合成
#define NUM_GRAMMAR_ENUM(num, str, id)          \
	n = strlen(str);                            \
	if (tmp.substr(0, n) == str)                \
	{                                           \
		tokens.push_back(Token(TokenCode::id)); \
		ans.push_back(Token(TokenCode::id));    \
		i += n - 1;                             \
		continue;                               \
	}
#include "../include/TokensMulOperator.inl"
#undef NUM_GRAMMAR_ENUM

			tokens.push_back(newtokens[i]);
			ans.push_back(newtokens[i]);
		}
	}
	// 最后加进来换行标志，方便形式化输出
	tokens.push_back(Token(TokenCode::ENDL));
	ans.push_back(Token(TokenCode::ENDL));
	return ans;
}

/***************************************************************************
  函数名称：Debug
  功    能：调试输出
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void Lexer::Debug()
{
	cout << "---------------------------LexerDebug" << endl;
	for (auto x : tokens)
	{
		x.Debug();
	}
	cout << "---------------------------LexerDebug" << endl;
}
