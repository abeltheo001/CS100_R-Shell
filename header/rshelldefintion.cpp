#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cassert>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "rshellclasses.h"
#include "rshellutils"
#include "rshellclasses.h"
#include "splitOnChar.h"
#include "groupQuotes.h"
#include "filterComments.h"
#include "tokenize.h"
#include "constructExpressionTree.h"

using namespace std;

/*
 * RShell::RShell(string input)
	{//Push into command Tree}

  virtual RShell::~RShell() {}
*/

void RShell::makeCommandTree(string userInput)
{
	if (DEBUG) {
		cout << "Starting parsing. Current working string/vector:" << endl;
		cout << userInput << endl;
	}

	// 0. Messy way to format ";" how we want - insert a space before every ;
	vector<string> test = splitOnChar(userInput,';');
	userInput = joinVector(test," ;"); 
	if (DEBUG) {
		cout << "Fixed spacing for ;. Current working string/vector:" << endl;
		cout << userInput << endl;
	}

	// 1. Split on spaces
	vector<string> words = splitOnChar(userInput, ' ');
	if (DEBUG) {
		cout << "Split on spaces. Current working string/vector:" << endl;
		printVector(words);
	}

 	// 2. Make sure quotes get grouped together
	words = groupQuotes(words);
	if (DEBUG) {
		cout << "Grouped quotes together. Current working string/vector:" << endl;
		cout << userInput << endl;
	}

	// 3. Filter out stuff after #
	words = filterComments(words);
	if (DEBUG) {
		cout << "Comments filtered out. Current working string/vector:" << endl;
		cout << words << endl;
	}

	// TODO: Merge tokenize and constructExpressionTree so all the new's are in one place

	// 4. Group words/quotes into Tokens, and merge them into a CommandTree.
	constructExpressionTree(words); // No returns since it constructs into our currentTree member variable

//	// 4. Group words/quotes into Tokens of type Subcommand or Operator.
//	vector<Token*> tokens = tokenize(words);
//
//	// 5. Create a tree for execution in later steps
//	constructExpressionTree(tokens);
}

int RShell::executeCommandTree()
{
	Token* head = currentTree->getHead();
	if (head != nullptr) {
		head->execute();
		if (DEBUG) {
			cout << "Execution complete! CommandTree post execution:" << endl;
			cout << currentTree->stringify() << endl;
		}
		return (currentTree->getHead())->status;
	} else {
		if (DEBUG) {
			cout << "CommandTree was empty, so nothing to run." << endl;
		}
		return 0;
	}

	delete currentTree;
}

