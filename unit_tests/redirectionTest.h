#ifndef _REDIRECT_TESTS
#define _REDIRECT_TESTS

#include "gtest/gtest.h"
#include "../header/rshellclasses.h"
#include "../header/rshelldefinition.h"
#include <stack>
#include <queue>
#include "../header/shuntingExecute.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

TEST (redirectOutTest, Appending)
{
	RShell shellobj = RShell();
	string file = "input.txt";

	Token* command = new Subcommand({file});
	Token* response = new Subcommand({"DM the god himself"});
	Token* check = new AppendOutToken({">>"});


	deque<Token*> V;
	V.push_back(command);
	V.push_back(response);
	V.push_back(check);

	shellobj.commandDeque = V;
	shellobj.shuntingExecute(V);
	
	string checker;
	ifstream in;
	in.open(file);
	getline(in,checker);

	

	EXPECT_EQ(checker,"DM the god himself");
}

TEST (redirectOutTest, Wipe)
{
	
}


#endif
