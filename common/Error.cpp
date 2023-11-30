/* Error.cpp */
#include "../include/Error.h"

/***************************************************************************
	函数名称：Error
	功    能：错误输出并中断程序
	输入参数：int, 错误原因编号
	返 回 值：
	说    明：输出错误信息，并直接退出程序
***************************************************************************/
void Error(int ret, int linenum)
{
	cout << "ERROR!!!   " << ret << endl;
	// 宏定义调用.inl文件
	switch (ret)
	{
#define ERROR_MESSAGE(num, str)                        \
	case num:                                          \
		if (linenum)                                   \
			cerr << "errorline : " << linenum << endl; \
		cerr << #str << endl;                          \
		break;
#include "../include/ErrorMessages.inl"
#undef ERROR_MESSAGE
	default:
		cerr << "未知错误!" << endl;
		break;
	}
	exit(ret);
}