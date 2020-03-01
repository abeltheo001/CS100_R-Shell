#ifndef SHUNTINGYARDCONSTRUCT_H
#define SHUNTINGYARDCONSTRUCT_H

#include <iostream>
#include <vector>
#include <deque>
#include <stack>
#include <unordered_map>
#include <unordered_set>

#include "rshellclasses.h"
#include "rshellutils.h"
#include "splitOnChar.h"

int RShell::findClose(const string& targetString, int start, char targetClose) {
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

deque<Token*> RShell::shuntingYardConstruct(string commandString) {
	// Want to go from input string to final deque to execute on
	// echo a || (( false && echo c ))
	// becomes
	// {Subcommand: {"echo", "a"},
	//  OrToken:    {"||"},
	//  ParenToken: leftChild->ParenToken,
	//  ParenToken:

	unordered_set<string> operators = {"||", "&&", ";"};
	unordered_map<char, char> openToClose = {
											{'(',')'},  
											{'[', ']'},
											{'"', '"'}
											};
	// The following deque is used for checking whether an operator has been read
	deque<char> backlog;
	int maxbacklog = 2; // Defined by the longest operator in operators; update as needed
	vector<int> allowed_lengths = {2, 1}; // Defined by length of each operator...
	// TODO: Assignment 4 will cause this approach to fail (probably)

	// The following vector is used for flushes to Subcommand
	vector<char> buffer;

	// Shunting yard deque/stack
	stack<Token*> shuntingSouth;
	deque<Token*> outputQueue;

	if (commandString.at(0) == '#') {
		return outputQueue;
	} else {
		int lastFlushed = 0;
		int currPos = 0;
		for (auto it = commandString.begin(); it != commandString.end(); it++) {
			// Check if currently on something in "operators"
			char c = *it;
			backlog.push_back(c);
			if (backlog.size() > maxbacklog) {
				backlog.pop_front();
			}

			bool opfound = false;
			if (currPos >= 4) { // Only makes sense to run this for "f ||" and larger
				string accepted = " ";
				int matchsize = -1;
				for (int i : allowed_lengths) {
					string query(backlog.begin()+(maxbacklog-i), backlog.end());
					if (operators.count(query) > 0) {
						accepted = query;
						matchsize = i;
						if (DEBUG == true) {
							cout << "backlog queue matched with: \"" << accepted << "\"" << endl;
						}
						break;
					}
				}

				// If _# is the furthest right, then we just started a comment block.
				string tworight(backlog.end()-2, backlog.end());
				if (tworight == " #") {
					break;
				}

				if (accepted != " ") {
					opfound = true;
					for (int i = 0; i < matchsize-1; i++) {
						buffer.pop_back();
					}
					string subcstring(buffer.begin(), buffer.end());
					// string subcstring = "echo hello";

					if (DEBUG == true) {
						cout << "generated subcommand string:" << subcstring << endl;
					}

					buffer.clear();
					vector<string> subcvect = splitOnChar(subcstring, ' '); // strip() and split(' ')!

					if (DEBUG == true) {
						cout << "generated subcommand vector:" << endl;
						printVector(subcvect,"; ");
					}

					lastFlushed = currPos + 1;

					if (subcvect.size() > 0) {
						// Needed for edge case of operator after ParenToken (should not insert " ")
						Subcommand* subcobj = new Subcommand(subcvect);
						outputQueue.push_back(subcobj);
					}

					// Construct specific type of Token
					Token* myToken;
					if (accepted == "||") {
						myToken = new OrToken({"||"});
					} else if (accepted == "&&") {
						myToken = new AndToken({"&&"});
					} else if (accepted == ";") {
						myToken = new SemiToken({";"});
					}

					if (DEBUG == true) {
						cout << "generated operator:" << myToken->stringify() << endl;
					}

					// In shunting yard, pop operators when a new one is added.
					while (shuntingSouth.size() > 0) {
						Token* myOp = shuntingSouth.top();
						shuntingSouth.pop();
						outputQueue.push_back(myOp);
					}
					shuntingSouth.push(myToken);
				}
			}


			if (opfound) {
				// Was already done in inner loop
			} else if (openToClose.count(c) > 0) { // It's something in the form (  ) or [   ] or "  "
				int closepos = findClose(commandString, currPos, openToClose[c]);
				string pairedstring(commandString.begin()+currPos+1, commandString.begin()+closepos);
				
				if (c == '[') {
					vector<string> testContent = splitOnChar(pairedstring,' ');
					Token* tToken;
					tToken = new TestToken(testContent);
					outputQueue.push_back(tToken);
				
				} else if (c == '(') {

					// Recursion time!
					// Why?
					// Shunting yard doesn't handle conditional execution for parentheses properly.
					// Example:
					// echo a || ( echo b && echo c )
					// Shunting yard would output this deque:
					// [echo a, echo b, echo c, &&, ||]
					// When thrown into execution,"d = echo b && echo c" would run first, and then "echo a || d".
					// While this is correct order of operations if this were algebra, we never wanted the stuff
					//     inside the parentheses to execute in the first place.
					// Therefore, I pair (   ) delimited stuff into a "ParenthesisToken", which causes Shunting Yard to produce something like this:
					// [echo a, [echo b, echo c, &&], ||]
					// This WILL be properly handled during execution, as opposed to the other deque.

					deque<Token*> retq = shuntingYardConstruct(pairedstring);
					ParenthesisToken* stuffInside = new ParenthesisToken(retq);

					outputQueue.push_back(stuffInside);
				
				} else if (c == '"') {

					// buffer.push_back(pairedstring);

				}



				// Now have to move iterator past the parenthesis end
				it += closepos-currPos;
				currPos += closepos-currPos;

			} else { // Not an operator or ()[]"", so it's part of a subcommand
				// string cstr;
				// cstr.push_back(c);
				buffer.push_back(c);
			}

			currPos += 1;
		}

		// Clear out leftover subcommand stuff
		if (buffer.size() > 0) {
			// No appending Subcommands after ParenTokens - this is an error
			if (outputQueue.size() > 0) {
				ParenthesisToken* ptest = dynamic_cast<ParenthesisToken*>(outputQueue[outputQueue.size()-1]);
				if (ptest != nullptr) {
					// TODO: throw error
				}
			}

			bool allspaces = false;
			for (char s : buffer) {
				if ((s != ' ') && (s != '\t')) {
					allspaces = false;
					break;
				}
			}

			if (!(allspaces)) {
				string finsubcstring(buffer.begin(), buffer.end());
				vector<string> finsubcvect = splitOnChar(finsubcstring, ' ');
				Subcommand* finsubcobj = new Subcommand(finsubcvect);
				outputQueue.push_back(finsubcobj);
			}
		}

		// Clear out leftover operators
		while (shuntingSouth.size() > 0) {
			outputQueue.push_back(shuntingSouth.top());
			shuntingSouth.pop();
		}

		return outputQueue;
	}
}

#endif
