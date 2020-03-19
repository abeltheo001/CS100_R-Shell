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

TEST (redirectTest, Appending)
{
	RShell shellobj = RShell();
	string file = "input.txt";

	//Create inputs. 
	Token* command = new Subcommand({"input.txt"});
	Token* response = new Subcommand({"echo a"});
	Token* check = new AppendOutToken({">>"});

	//Store inputs into deque
	deque<Token*> V;
	V.push_back(response);
	V.push_back(command);
	V.push_back(check);

	//Run the shell
	shellobj.commandDeque = V;
	shellobj.shuntingExecute(V);
	
	//Open the file 
	ifstream in;
	in.open(file);

	//Check bottom of file for latest input
	//Iterator to check bottom row of file based on code from stackoverflow
	//https://stackoverflow.com/questions/11876290/c-fastest-way-to-read-only-last-line-of-text-file
	if (in.is_open()){	
		char x; 
		bool check = true;
		while(check) {
		if ((int)in.tellg() <= 1) {
			in.seekg(0);
			check = false;
		} else if (x == '\n') {
			check = false;
		} else {
			in.seekg(-2, ios_base::cur);
		} }

	}	

	//Once at the lowest point in the file, grab the string and check if it's correct.
	string end;
	getline (in,end);
	in.close();	
	

	EXPECT_EQ(end,"a");
}

TEST (redirectTest, Wipe)
{
	
}

TEST(redirectTest, Pipe)
{
}


#endif
