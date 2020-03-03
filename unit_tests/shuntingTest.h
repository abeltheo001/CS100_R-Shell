#ifndef _SHUNTING_TEST_H
#define _SHUNTING_TEST_H

#include "gtest/gtest.h"
#include "../header/rshellclasses.h"
#include "../header/rshelldefinition.h"
#include "../header/shuntingYardConstruct.h"
#include "../header/shuntingExecute.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

TEST(constructorTest,singleEcho) {
	string input = "echo a";
   	RShell rshellobj = RShell(false);
	deque<Token*> commandDeque = rshellobj.shuntingYardConstruct(input);

	deque<Token*> correct;
	vector<string> V = {"echo", "a"};
	Subcommand* a = new Subcommand(V);
	correct.push_back(a);

	ASSERT_EQ(correct.size(),commandDeque.size());
	delete a;
}
	

TEST (executorTest, execution)
{
	deque<Token*> result;
	vector<string> V = {"echo","b"};
	Subcommand* a = new Subcommand(V);
	result.push_back(a);

	RShell rshellobj = RShell(false);
	int functionResult = rshellobj.shuntingExecute(result);
	
	ASSERT_EQ(functionResult, 0);
	delete a;
}
/*
 * Remaining tests:
 * void makeCommandDeque(string);
 * deque<Token*> shuntingYardConstruct(string);
 * int findClose(const string&,int,char); 
 * */

TEST (DequeCreatorTest,DequeCreator)
{
	string input = "echo a || echo b && echo c";
	RShell shell = RShell(false);
	shell.makeCommandDeque(input);
	deque<Token*> result;
	result = shell.commandDeque;

	deque<Token*> check; 
	vector<string> sub = {"echo", "a"};
	Subcommand* a = new Subcommand(sub);
	check.push_back(a);
	vector<string> sub2 = {"echo", "b"};
	Subcommand* b = new Subcommand(sub2);
	check.push_back(b);
	Token* myToken = new OrToken({"||"});
	check.push_back(myToken);
	vector<string> sub3 = {"echo","c"};
	Subcommand* c = new Subcommand(sub3);
	check.push_back(c);
	Token* addToken = new AndToken({"&&"});
	check.push_back(addToken);

	ASSERT_EQ(result.size(),check.size());
	delete a, b, c;
}
#endif
