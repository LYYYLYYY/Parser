/* TestGrammarMain.cpp */
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "../include/Grammar2Tables.h"
using namespace std;

int main()
{
	string filename="../files/grammar.txt";
	// string filename="test1.txt";
	// string filename="grammar3.txt";
	// map<string,int> Tokens=GetTokens(filename);
	// for(auto x:Tokens)
	// {
	// 	cout<<x.first<<' '<<x.second<<endl;
	// }
	// vector<pair<string,vector<string>>> Generates=GetGenerates(filename);
	// for(auto x:Generates)
	// {
	// 	if(x.first==x.second[0]) 
	// 	{
	// 		cout<<x.first<<':';
	// 		for(auto y:x.second) cout<<y<<' ';
	// 		cout<<endl;
	// 		puts("NONONONO");
	// 	}
	// }
	// for(auto x:Generates)
	// {
	// 	cout<<x.first<<':';
	// 	for(auto y:x.second) cout<<y<<' ';
	// 	cout<<endl;
	// }
	// map<string,vector<string>> FIRST=GetFIRST(Tokens,Generates);
	// for(auto x:FIRST)
	// {
	// 	cout<<x.first<<':';
	// 	for(auto y:x.second) cout<<y<<' ';
	// 	cout<<endl;
	// }
	// map<string,vector<string>> FOLLOW=GetFOLLOW(FIRST,Generates);
	// for(auto x:FOLLOW)
	// {
	// 	cout<<x.first<<':';
	// 	for(auto y:x.second) cout<<y<<' ';
	// 	cout<<endl;
	// }
	// vector<pair<pair<string,vector<string>>,unsigned int>> LR0Projects=GetLR0Projects(Generates);
	// for(auto x:LR0Projects)
	// {
	// 	cout<<x.first.first<<' '<<x.second<<':';
	// 	for(auto y:x.first.second) cout<<y<<' ';
	// 	cout<<'\n';
	// }
	// pair<vector<set<pair<pair<string,vector<string>>,pair<unsigned int,string>>>>,map<pair<int,string>,int>> tmp=GetClosuresAndGo(Generates,FIRST);
	// vector<set<pair<pair<string,vector<string>>,pair<unsigned int,string>>>> Closures=tmp.first;
	// cout<<Closures.size()<<endl;
	// map<pair<int,string>,int> GO=tmp.second;
	// freopen("closures.txt","w",stdout);
	// for(auto x:Go)
	// {
	// 	cout<<x.first.first<<' '<<x.first.second<<':'<<x.second<<endl;
	// }
	Grammar2Tables test(filename);
	test.DebugGrammarTokens("../files/Tokens.txt");
	test.DebugGrammarGenerates("../files/Generates.txt");
	// test.DebugGrammarFIRST();
	// test.DebugGrammarFOLLOW();
	// test.DebugGrammarLR0Projects();
	test.DebugGrammarClosures("../files/Closures.txt");
	// test.DebugGrammarClosures();
	// test.DebugGrammarGo();
	test.CheckIsLR1("../files/GrammarConflicts.txt");
	test.DebugGrammarACTIONAndGOTOprivate("../files/ACTIONAndGOTOPrivate.txt");
	test.DebugGrammarACTIONAndGOTO("../files/ACTIONAndGOTO.csv");
	return 0;
}
