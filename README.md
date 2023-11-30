# 同济大学计算机系 TJCS 编译原理语法分析大作业

## 程序架构

文法预处理: TestGrammarMain.cpp

	输入 grammar.txt 
	输出 Generates.txt ACTIONAndGOTOPrivate.txt

	grammar.txt: 文法描述文件
	Generates.txt: 产生式描述文件
	ACTIONAndGOTOPrivate.txt: ACTION表和GOTO表描述文件

	当前目录下的编译参数(供参考): c++ TestGrammarMain.cpp Grammar2Tables.cpp Error.cpp -o TestGrammarMain -Wall -Wl,--stack=1926091777 -O2 -std=c++14

语法分析(含此法分析): TestParserMain.cpp

	输入 Generates.txt ACTIONAndGOTOPrivate.txt C语言源文件
	输出 nodes.txt actions.json
	
	Generates.txt: 产生式描述文件
	ACTIONAndGOTOPrivate.txt: ACTION表和GOTO表描述文件
	C语言源文件: 待分析的C语言文件
	nodes.txt: 语法分析树描述文件
	actions.json: 语法分析时栈的行为文件

	当前目录下的编译参数(供参考): c++ TestParserMain.cpp Parser.cpp Lexer.cpp Error.cpp Token.cpp -o TestParserMain -Wall -Wl,--stack=1926091777 -O2 -std=c++14

界面: TestVisualMain.py

	输入 nodes.txt actions.json StackVisualization0.html
	输出 tree_visualization.png StackVisualization.html
	
	nodes.txt: 语法分析树描述文件
	actions.json: 语法分析时栈的行为文件
	StackVisualization0.html: 空栈的图形化展示文件
	tree_visualization.png: 语法分析树的图形化展示图片
	StackVisualization.html: 语法分析栈的图形化展示文件

	当前目录下的编译参数(供参考): python -u TestVisualMain.py

	python 环境(供参考):
	python 3.9.18
	graphviz 2.50.0
	json5 0.9.14
	
