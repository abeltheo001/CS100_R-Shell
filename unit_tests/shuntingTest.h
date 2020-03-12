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

	// Construct AndTokens
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

	for (Token* t : correct) {
		delete t;
	}
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

	for (Token* t : check) {
		delete t;
	}
}	

TEST (shuntingExecute, singleEcho)
{
	deque<Token*> result;
	vector<string> V = {"echo", "a"};
	Subcommand* a = new Subcommand(V);
	result.push_back(a);

	RShell rshellobj = RShell(false);
	rshellobj.shuntingExecute(result);

	vector<int> expectation = {0};
	
	ASSERT_EQ(result.size(), expectation.size()); // If this fails, you wrote the test wrong

	bool equal = true;
	for (int i = 0; i < expectation.size(); i++) {
		if (expectation[i] != result[i]->status) {
			equal = false;
			cout << "Status was not as expected:" << endl;
			cout << expectation[i] << " != " << result[i]->status << endl;
			cout << "Happened on command " << result[i]->stringify() << endl;
		}
	}

	EXPECT_EQ(equal, true);

	delete a;
}

// TEST (shuntingExecute, bigger)
// {
// 	// "echo a || [ -e CMakeLists.txt ] && (           echo c && echo d)"

// 	// First command from:
// 	// https://codegolf.stackexchange.com/a/4403

// 		// Meanings of status:
// 		// -3: Subcommand segfaulted when we tried to run it with execvp
// 		// -2: Token has not run
// 		// -1: Subcommand not found
// 		// 0: Subcommand ran successfully
// 		// anything else: Subcommand failed

// 	// Should return {0, -2, 0, 1, 1} with the inside of ParenthesisToken being {-1, -2, 1}

// 	// Construct Subcommand
// 	vector<string> V = {"echo a"};
// 	Subcommand* t1 = new Subcommand(V);

// 	// Construct TestToken
// 	V = {"-e", "CMakeLists.txt"};
// 	TestToken* t2 = new TestToken(V);

// 	// Construct ParenToken
// 	V = {"echo", "c"};
// 	Subcommand* s1 = new Subcommand(V);
// 	V = {"echo", "d"};
// 	Subcommand* s2 = new Subcommand(V);
// 	V = {"||"};
// 	OrToken* s3 = new OrToken(V);
// 	deque<Token*> intermediate;
// 	intermediate.push_back(s1);
// 	intermediate.push_back(s2);
// 	intermediate.push_back(s3);
// 	ParenthesisToken* t3 = new ParenthesisToken(intermediate);

// 	// Construct And/Or Tokens
// 	V = {"||"};
// 	OrToken* t4 = new OrToken(V);
// 	V = {"&&"};
// 	AndToken* t5 = new AndToken(V);

// 	deque<Token*> result;
// 	result.push_back(t1); // echo a
// 	result.push_back(t2); // -e CMakeLists.txt
// 	result.push_back(t4); // ||
// 	result.push_back(t3); // ParenthesisToken
// 	result.push_back(t5); // &&

// 	RShell rshellobj = RShell(false);
// 	rshellobj.shuntingExecute(result);

// 	vector<int> expectation = {0, -2, 0, 1, 1};
	
// 	ASSERT_EQ(result.size(), expectation.size()); // If this fails, you wrote the test wrong

// 	bool equal = true;
// 	for (int i = 0; i < expectation.size(); i++) {
// 		if (expectation[i] != result[i]->status) {
// 			equal = false;
// 			cout << "{" << endl;
// 			cout << "Status was not as expected (expected on left):" << endl;
// 			cout << expectation[i] << " != " << result[i]->status << endl;
// 			cout << "Happened on command " << result[i]->stringify() << endl;
// 			cout << "}" << endl;
// 		}
// 	}

// 	EXPECT_EQ(equal, true);

// 	delete t1, t2, t3, t4, t5;
// }

// TEST (shuntingExecute, mostConditionCodes)
// {
// 	// "echa || [ -e CMakeLists.txt ] && (           echa && echo d)"

// 	// First command from:
// 	// https://codegolf.stackexchange.com/a/4403

// 		// Meanings of status:
// 		// -3: Subcommand segfaulted when we tried to run it with execvp
// 		// -2: Token has not run
// 		// -1: Subcommand not found
// 		// 0: Subcommand ran successfully
// 		// anything else: Subcommand failed

// 	// Should return {-1, 0, 0, 1, 1} with the inside of ParenthesisToken being {-1, -2, 1}

// 	// Construct Subcommand
// 	vector<string> V = {"echa"};
// 	Subcommand* t1 = new Subcommand(V);

// 	// Construct TestToken
// 	V = {"-e", "CMakeLists.txt"};
// 	TestToken* t2 = new TestToken(V);

// 	// Construct ParenToken
// 	V = {"echa"};
// 	Subcommand* s1 = new Subcommand(V);
// 	V = {"echo", "d"};
// 	Subcommand* s2 = new Subcommand(V);
// 	V = {"||"};
// 	OrToken* s3 = new OrToken(V);
// 	deque<Token*> intermediate;
// 	intermediate.push_back(s1);
// 	intermediate.push_back(s2);
// 	intermediate.push_back(s3);
// 	ParenthesisToken* t3 = new ParenthesisToken(intermediate);

// 	// Construct And/Or Tokens
// 	V = {"||"};
// 	OrToken* t4 = new OrToken(V);
// 	V = {"&&"};
// 	AndToken* t5 = new AndToken(V);

// 	deque<Token*> result;
// 	result.push_back(t1); // kill -11 $$
// 	result.push_back(t2); // -e CMakeLists.txt
// 	result.push_back(t4); // ||
// 	result.push_back(t3); // ParenthesisToken
// 	result.push_back(t5); // &&

// 	RShell rshellobj = RShell(false);
// 	rshellobj.shuntingExecute(result);

// 	vector<int> expectation = {-1, 0, 0, 1, 1};
	
// 	ASSERT_EQ(result.size(), expectation.size()); // If this fails, you wrote the test wrong

// 	bool equal = true;
// 	for (int i = 0; i < expectation.size(); i++) {
// 		if (expectation[i] != result[i]->status) {
// 			equal = false;
// 			cout << "{" << endl;
// 			cout << "Status was not as expected (expected on left):" << endl;
// 			cout << expectation[i] << " != " << result[i]->status << endl;
// 			cout << "Happened on command " << result[i]->stringify() << endl;
// 			cout << "}" << endl;
// 		}
// 	}

// 	EXPECT_EQ(equal, true);

// 	delete t1, t2, t3, t4, t5;
// }

// TEST (shuntingExecute, notFound)
// {
// 	// Construct Subcommand
// 	vector<string> V = {"echa"};
// 	Subcommand* t1 = new Subcommand(V);

// 	deque<Token*> result;
// 	result.push_back(t1); // invalid

// 	RShell rshellobj = RShell(false);
// 	rshellobj.shuntingExecute(result);

// 	vector<int> expectation = {-1};
	
// 	ASSERT_EQ(result.size(), expectation.size()); // If this fails, you wrote the test wrong

// 	bool equal = true;
// 	for (int i = 0; i < expectation.size(); i++) {
// 		if (expectation[i] != result[i]->status) {
// 			equal = false;
// 			cout << "{" << endl;
// 			cout << "Status was not as expected (expected on left):" << endl;
// 			cout << expectation[i] << " != " << result[i]->status << endl;
// 			cout << "Happened on command " << result[i]->stringify() << endl;
// 			cout << "}" << endl;
// 		}
// 	}

// 	EXPECT_EQ(equal, true);

// 	delete t1;
// }

TEST (executorTest, DequeExecutor)
{
	RShell input = RShell(false);
	input.makeCommandDeque("echo a || (false && echo c)");
	int result = input.executeCommandDeque();
	
	ASSERT_EQ(0,result);
}

TEST (findCloseTest, oneLayerCheck)
{
	string input = "echo a || (false && echo c)";
	int start = 10;
	char targetClose = ')';
	RShell shell = RShell(false);
	int result = shell.findClose(input,start,targetClose);

	EXPECT_EQ(result, 26);
}

TEST (findCloseTest, multiLayerCheck)
{
	string input = "echo a || ((  ((  ( (false && echo c)) ) )   ) )";
	int start = 10;
	char targetClose = ')';
	RShell shell = RShell(false);
	int result = shell.findClose(input,start,targetClose);

	EXPECT_EQ(result, 47);
}

#endif
