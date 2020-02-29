#ifndef SHUNTINGYARDCONSTRUCT_H
#define SHUNTINGYARDCONSTRUCT_H

#include <iostream>
#include <vector>
#include <queue>
#include <stack>

#include "rshellclasses.h"
#include "rshellutils.h"

using namespace std;

int findClose(string& targetString, int start, char targetClose) {
	// Given this:
	//              |
	// mgoiejnoaijt ( kteopajtio []gea][]g[a((()))(()) )
	// Where the | represents the start char pos, and targetclose = ')',
	// then findClose will return:
	//                                                 |
	// mgoiejnoaijt ( kteopajtio []gea][]g[a((()))(()) )
	//
	// This allows for the range of ParenthesisToken to be defined.
	int depth = 0;
	char open = targetString.at(start);
	for (int i = start+1; i < targetString.size(); i++) {
		char currChar = targetString.at(i);
		if (currChar == open) {
			depth += 1;
		} else if ((currChar == targetClose) && (depth > 0)) {
			depth -= 1;
		} else if ((currChar == targetClose) && (depth == 0)) {
			return i;
		}
	}

	return -1;
}

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
