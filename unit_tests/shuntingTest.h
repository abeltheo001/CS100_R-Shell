#ifndef _SHUNTING_TEST_H
#define _SHUNTING_TEST_H

#include <iostream>
#include <string>
#include <vector>
#include "gtest/gtest.h"

#include "../header/rshellclasses.h"
#include "../header/rshelldefinition.h"
#include "../header/shuntingYardConstruct.h"
#include "../header/shuntingExecute.h"

using namespace std;

TEST(shuntingYardConstruct, singleEcho) {
	string input = "echo a";
   	RShell rshellobj = RShell(false);
	deque<Token*> commandDeque = rshellobj.shuntingYardConstruct(input);

	deque<Token*> correct;
	vector<string> V = {"echo", "a"};
	Subcommand* a = new Subcommand(V);
	correct.push_back(a);

	ASSERT_EQ(correct.size(),commandDeque.size());

	bool equal = true;
	for (int i = 0; i < commandDeque.size(); i++) {
		if (!(*(commandDeque[i]) == *(correct[i]))) {
			equal = false;
			cout << "Equality check failed at position: " << i << endl;
			cout << commandDeque[i]->stringify() << " is not equal to " << correct[i]->stringify() << endl;
		}
	}

	EXPECT_EQ(equal, true);

	delete a;
}

TEST(shuntingYardConstruct, parenthesesAndTest) {
	string input = "echo a && [ -e CMakeLists.txt ] && (echo c || echo d)";
   	RShell rshellobj = RShell(false);
	deque<Token*> commandDeque = rshellobj.shuntingYardConstruct(input);

	// Construct Subcommand
	vector<string> V = {"echo", "a"};
	Subcommand* t1 = new Subcommand(V);

	// Construct TestToken
	V = {"-e", "CMakeLists.txt"};
	TestToken* t2 = new TestToken(V);

	// Construct ParenToken
	V = {"echo", "c"};
	Subcommand* s1 = new Subcommand(V);
	V = {"echo", "d"};
	Subcommand* s2 = new Subcommand(V);
	V = {"||"};
	OrToken* s3 = new OrToken(V);
	deque<Token*> intermediate;
	intermediate.push_back(s1);
	intermediate.push_back(s2);
	intermediate.push_back(s3);
	ParenthesisToken* t3 = new ParenthesisToken(intermediate);

	V = {"&&"};
	AndToken* t4 = new AndToken(V);
	V = {"&&"};
	AndToken* t5 = new AndToken(V);

	deque<Token*> correct;
	correct.push_back(t1); // echo a
	correct.push_back(t2); // -e CMakeLists.txt
	correct.push_back(t4); // &&
	correct.push_back(t3); // ParenthesisToken
	correct.push_back(t5); // &&

	ASSERT_EQ(correct.size(),commandDeque.size());

	bool equal = true;
	for (int i = 0; i < commandDeque.size(); i++) {
		if (!(*(commandDeque[i]) == *(correct[i]))) {
			equal = false;
			cout << "Equality check failed at position: " << i << endl;
			cout << commandDeque[i]->stringify() << " is not equal to " << correct[i]->stringify() << endl;
		}
	}

	EXPECT_EQ(equal, true);

	delete t1, t2, t3, t4, t5;
}

TEST (makeCommandDeque, bashConnectors)
{
	// Basically the same as shuntingYardConstruct(), so not really anything new to test

	string input = "echo a || echo b && echo c";
	RShell shell = RShell(false);
	shell.makeCommandDeque(input);

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

	ASSERT_EQ((shell.commandDeque).size(),check.size());

	delete a, b, c;
}	

TEST (shuntingExecute, execution)
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

#endif
