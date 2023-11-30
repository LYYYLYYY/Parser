/* Token.h 表示token */

#pragma once

#include <iostream>
#include <string>

using namespace std;

// 宏定义调用.inl文件
// 详情参考 https://www.zhihu.com/question/482502037
/***************************************************************************
	类名：TokenCode
	功能：表示不同的终结符
	成员：100~199-保留字, 200~299-单字符符号, 300~399-多字符符号, 0~99-其它
	说明：枚举类，可以在编译阶段将文件中的内容通过宏定义的方式写入枚举类中。放入文件中，方便后续修改
***************************************************************************/
enum class TokenCode
{
#define NUM_GRAMMAR_ENUM(num, str, id) id = num,
#include "Tokens.inl"
#undef NUM_GRAMMAR_ENUM
};

/***************************************************************************
	类名：Token
	功能：表示一个token
	成员：type-token类型, pos-token在符号表或常数表中的位置
	说明：如果不是标识符或者常数则pos=-1
***************************************************************************/
class Token
{
private:
	TokenCode type;
	int pos;

public:
	Token(const TokenCode &op) : type(op), pos(-1) {}
	Token(const TokenCode &op, int pos) : type(op), pos(pos) {}
	TokenCode GetType(void);
	string Type2String(void);
	void Debug(void);
};
