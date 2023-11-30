/* Token.cpp */
#include "../include/Token.h"

/***************************************************************************
  函数名称：GetType
  功    能：获得token的类型
  输入参数：
  返 回 值：TokenCode
  说    明：
***************************************************************************/
TokenCode Token::GetType()
{
	return type;
}

string Token::Type2String()
{
	// 宏定义调用.inl文件
	switch (type)
	{
#define NUM_GRAMMAR_ENUM(num, str, id) \
	case TokenCode::id:                \
		return #str;
#include "../include/Tokens.inl"
#undef NUM_GRAMMAR_ENUM
	default:
		return "";
	}
	return "";
}

/***************************************************************************
  函数名称：Debug
  功    能：Token调试输出
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void Token::Debug()
{
	string nw = Type2String();
	cout << (nw == "Endl" ? "\n" : nw + ' ');
	return;
	cout << static_cast<int>(type) << ' ' << pos << ' ' << nw << endl;
}