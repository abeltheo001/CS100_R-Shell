#ifndef SHUNTINGYARDCONSTRUCT_H
#define SHUNTINGYARDCONSTRUCT_H

#include <iostream>
#include <vector>
#include <deque>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <iterator>

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

		if ((currChar == targetClose) && (depth > 0)) {
			if (DEBUG) {
				cout << "Found another " << open << " at pos " << i << "." << endl;
			}
			depth -= 1;
		} else if ((currChar == targetClose) && (depth == 0)) {
			return i;
		} else if (currChar == open) {
			depth += 1;
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

	unordered_set<string> operators = {"||", "&&", ";" ,">", "<", ">>", "|"};
	unordered_map<char, char> openToClose = {
											{'(',')'},  
											{'[', ']'},
											{'\"', '\"'}
											};
	unordered_set<char> closes;
	for (pair<char, char> cpair : openToClose) {
		closes.insert(cpair.second);
	}
	closes.erase('\"'); // This will be handled by findCloses later on.

	// Prepare parser for information in operators
	// 1. Maximum size of deque to look back
	// 2. Allowed lengths to check (is 1 back this? is 2 back this?)
	// 3. Collisions requiring lookahead (eg > and >>)

	// The following deque is used for checking whether an operator has been read
	deque<char> backlog;
	map<int, vector<string>> operatorlengths; // By default this is already sorted by key on inserts :)

	// Set (1) and (2)
	int maxbacklog = 0;
	set<int> allowed_lengths;
	for (string s : operators) {
		if (s.size() > maxbacklog) {
			maxbacklog = s.size();
		}
		allowed_lengths.insert(s.size());
		if (operatorlengths.count(s.size()) > 0) {
			operatorlengths[s.size()].push_back(s);
		} else {
			pair<int, vector<string>> toInsert = {s.size(), {s}};
			operatorlengths.insert(toInsert);
		}
	}

	// Find collisions (eg | and ||) (ie set (3))
	// If any operators match at any end position, (eg | and || match at index 0, which is the end of |), then there's a collision
	// Simplest as an n^2 algorithm
	unordered_map<string, string> collisions;
	for (auto it = allowed_lengths.begin(); it != allowed_lengths.end(); it++) {
		if (*it != maxbacklog) {
			for (string op_small : operatorlengths[*it]) {
				auto it2 = it;
				it2++;
				while (it2 != allowed_lengths.end()) {
					for (string op_large : operatorlengths[*it2]) {
						// cout << op_small << " " << op_large.substr(0,*it) << endl;
						if (op_small == op_large.substr(0,*it)) {
							pair<string, string> col = {op_small, op_large};
							collisions.insert(col);

						}
					}
					it2++;
				}
			}
		}
	}

	if (DEBUG) {
		cout << "Detected collisions: " << collisions.size() << endl;
		for (auto p : collisions) {
			cout << p.first << " " << p.second << endl;
		}
	}

	// The following vector is used for flushes to Subcommand
	vector<string> buffer;

	// Shunting yard deque/stack
	stack<Token*> shuntingSouth;
	deque<Token*> outputQueue;

	// Cases for shuntingYardConstruct.h:
	// 0. If the string starts with #, then it's empty. Return an empty deque.
	// 1. The current backlog is _#. If so, just make a Subcommand if there's anything meaningful 
	//    on the buffer, and then return outputQueue.
	// 2. If the backlog queue matches a defined operator, then construct an operator and
	//    shunting push it onto the stack.
	//    In some special cases, eg | and ||, there can be collisions, but these are detected prior
	//    to runtime and handled by looking ahead one character.
	// 3. If the current character matches something in openToClose (eg ( ) or [ ]), then the entire
	//    inside of the block needs to be dealt with together. For ( ), shuntingYardConstruct is
	//    called recursively. For [ ], the insides are split on spaces and sent to TestToken. For
	//    quotes, the entire inside is turned into a string, and then pushed onto the buffer.
	// 4. If none of the above conditions applied, then this char is probably part of a Subcommand.
	//    Push it to the buffer. (The multi-char operators deal with this by deleting stuff from the
	//    buffer as needed.)

	if (commandString.at(0) == '#') {
		return outputQueue;
	} else {
		int currPos = 0;
		for (auto it = commandString.begin(); it != commandString.end(); it++) {
			// Check if currently on something in "operators"
			char c = *it;
			if (DEBUG) {
				cout << "currChar: " << c << endl;
			}
			backlog.push_back(c);
			if (backlog.size() > maxbacklog) {
				backlog.pop_front();
			}

			// If _# is the furthest right, then we just started a comment block.
			if (backlog.size() > 1) {
				string tworight(backlog.end()-2, backlog.end());
				if (tworight == " #") {
					break;
				}
			}

			bool opfound = false;
			if (currPos >= 2) { // Only makes sense to run this block (checks for operators) for "f ||" and larger
				// Look through operator list and see if there's a match
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

				if (accepted != " ") {
					opfound = true;
					for (int i = 0; i < matchsize-1; i++) {
						buffer.pop_back();
					}

					// Create Subcommand string by merging vector<string> buffer
					stringstream res;
					copy(buffer.begin(), buffer.end(), ostream_iterator<string>(res));
					string subcstring = res.str();
					// string subcstring = "echo hello";

					if (DEBUG == true) {
						cout << "generated subcommand string:" << subcstring << endl;
					}

					// Split Subcommand string
					buffer.clear();
					vector<string> subcvect = splitOnChar(subcstring, ' '); // strip() and split(' ')!

					if (DEBUG == true) {
						cout << "generated subcommand vector:" << endl;
						printVector(subcvect,"; ");
					}

					// Actually generate Subcommand vector
					if (subcvect.size() > 0) {
						// Needed for edge case of operator after ParenToken (should not insert " ")
						Subcommand* subcobj = new Subcommand(subcvect);
						outputQueue.push_back(subcobj);
					}

					// Run collision checks to see if we need to do > or >>
					if (collisions.count(accepted) > 0) {
						string larger = collisions[accepted];
						int diff = larger.size() - accepted.size();
						if (currPos + diff >= commandString.size()) {
							cout << "ERROR: There appears to be an operator at the end of your commandString. Terminating." << endl;
							for (Token* t : outputQueue) {
								delete t;
							}
							outputQueue.clear();
							return outputQueue;
						} else {
							// Either it's > or >> at this point
							bool allmatch = true;
							int largerpos = accepted.size();
							for (int i = currPos + accepted.size(); i < currPos + diff; i++) {
								if (commandString[i] != larger[largerpos]) {
									allmatch = false;
									break;
								}
							}

							// If not everything matches, just keep the old accepted string.
							// Otherwise update accordingly.
							if (allmatch == true) {
								accepted = larger;
								currPos += diff;
								it += diff;
							}
						}
					}

					// Construct our specific type of Token
					Token* myToken;
					if (accepted == "||") {
						myToken = new OrToken({"||"});
					} else if (accepted == "&&") {
						myToken = new AndToken({"&&"});
					} else if (accepted == ";") {
						myToken = new SemiToken({";"});
					} else if (accepted == "<") {
						myToken = new RedirectInputToken({"<"});
					} else if (accepted == ">>") {
						myToken = new RedirectOutToken({">>"});
					} else if (accepted == ">") {
						myToken = new RedirectOutToken({">"});
					} else if (accepted == "|") {
						myToken = new PipeToken({"|"});
					}
					
					if (DEBUG == true) {
						cout << "generated operator:" << myToken->stringify() << endl;
					}

					// In Shunting Yard, pop operators when a new one is added.
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
			} else if (closes.count(c) > 0) {
				cout << "RSHELL error: mismatched " << c << ". Exiting back to command loop." << endl;
				for (Token* t : outputQueue) {
					delete t;
				}
				outputQueue.clear();
				return outputQueue;
			} else if (openToClose.count(c) > 0) { // It's something in the form (  ) or [   ] or "  "
				int closepos = findClose(commandString, currPos, openToClose[c]);
				if (closepos == -1) {
					cout << "RSHELL error: mismatched " << c << ". Exiting back to command loop." << endl;
					for (Token* t : outputQueue) {
						delete t;
					}
					outputQueue.clear();
					return outputQueue;
				} else {

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
						buffer.push_back(pairedstring);
					}

					// Now have to move iterator past the parenthesis end
					it += closepos-currPos;
					currPos += closepos-currPos;
				}
			} else { // Not an operator or ()[]"", so it's part of a subcommand
				string cstr(1, c);
				buffer.push_back(cstr);
			}

			currPos += 1;
		}
	}

	// Clear out leftover subcommand stuff
	if (buffer.size() > 0) {
		// No appending Subcommands after ParenTokens - this is an error
		if (DEBUG) {
			cout << "some chars or strings are still in the buffer" << endl;
			printVector(buffer, "; ");
		}

		if (outputQueue.size() > 0) {
			ParenthesisToken* ptest = dynamic_cast<ParenthesisToken*>(outputQueue[outputQueue.size()-1]);
			if (ptest != nullptr) {
				// TODO: throw error - this is a command of syntax "(echo a) somestuffafterparen"
			}
		}

		bool allspaces = true;
		for (string s : buffer) {
			if ((s != " ") && (s != "\t")) {
				allspaces = false;
				break;
			}
		}

		if (!(allspaces)) {
			stringstream finres;
			copy(buffer.begin(), buffer.end(), ostream_iterator<string>(finres));
			string finsubcstring = finres.str();
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

#endif
