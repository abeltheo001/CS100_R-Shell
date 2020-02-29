#ifndef SHUNTINGYARDCONSTRUCT_H
#define SHUNTINGYARDCONSTRUCT_H

#include <iostream>
#include <vector>
#include <queue>
#include <stack>

#include "rshellclasses.h"
#include "rshellutils.h"

using namespace std;

void RShell::shuntingYardConstruct(string commandString) {
	// Want to go from input string to final queue to execute on
	// echo a || (( false && echo c ))
	// becomes
	// {Subcommand: {"echo", "a"},
	//  OrToken:    {"||"},
	//  ParenToken: leftChild->ParenToken,
	//  ParenToken: 

	vector<string> operators = {"||", "&&", ";"};
	vector<string> special = {"(", ")", "[", "]"};
	


}

#endif
