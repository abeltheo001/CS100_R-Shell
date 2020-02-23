#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cassert>
#include <queue>
#include <unordered_map>
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
 * Rshell::Rshell(string input)
	{//Push into command Tree}

  virtual Rshell::~Rshell() {}
*/

void Rshell::makeCommandTree(string input)
{
	vector<string> test = splitOnChar(userInput,';');
	userInput = joinVector(test," ;"); 

	// 1. Split on spaces
		vector<string> words = splitOnChar(userInput, ' ');
 	// 2. Make sure quotes get grouped together
		words = groupQuotes(words);

	// 3. Filter out 					
		words = filterComments(words);

	// 4. Group words/quotes into Tokens of type Subcommand or Operator.
 		vector<Token*> tokens = tokenize(words);

	// 5. Create a tree for execution in later steps
		currentTree = constructExpressionTree(tokens);
}

int Rshell::executeCommand(CommandTree& currentTree)
{
	Token* head = currentTree.getHead();
	if (head != nullptr) {
		head->execute();
		return (currentTree.getHead())->status;
	} else {
		return 0;
	}
	
}

