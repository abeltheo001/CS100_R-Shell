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
	Token* command2 = new Subcommand({"input.txt"});
	Token* command = new Subcommand({"echo cc"});
	Token* check = new AppendOutToken({">>"});


	//Store inputs into deque
	deque<Token*> V;
	V.push_back(command);
	V.push_back(command2);
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
	

	EXPECT_EQ(end,"cc");
}

TEST (redirectTest, Wipe)
{
	RShell shell = RShell();
	string file = "input.txt";


	//Create inputs. 
	Token* command = new Subcommand({"input.txt"});
	Token* response = new Subcommand({"echo a"});
	Token* check = new EmptyOutToken({">"});

	//Store inputs into deque
	deque<Token*> V;
	V.push_back(response);
	V.push_back(command);
	V.push_back(check);

	//Run the shell
	shell.commandDeque = V;
	shell.shuntingExecute(V);
	
	//Open the file 
	ifstream in;
	in.open(file);
	
	//Check the first line of the file. 
	string firstLine;
	if (in.is_open())
	{
		getline(in,firstLine);
	}
	in.close();
	
	EXPECT_EQ(firstLine,"a");

	
}

TEST(redirectTest, Pipe)
{
	RShell shell = RShell();
	string file = "newInput.txt";
		
	//Create inputs. 
	Token* echo = new Subcommand({"echo b > newInput.txt | tr a-z A-Z"});
	Token* input = new Subcommand({"newInput.txt"}); 
	Token* check = new EmptyOutToken({">"});


	//Store inputs to deque
	deque<Token*> V = {echo, input, check};

	//Run the shell
	shell.commandDeque = V;
	shell.shuntingExecute(V);

	
	//Open the file 
	ifstream in;
	in.open(file);
	
	//Check the first line of the file. 
	string firstLine;
	if (in.is_open())
	{
		getline(in,firstLine);
	}
	in.close();	

	
	EXPECT_EQ(firstLine,"B");

	
}


#endif
