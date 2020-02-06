#include <iostream>
#include "../header/rshellclasses.h"
#include "../header/splitSpaces.h"
#include "../header/filterComments.h" // TODO: Rename
// #include "tokenize.h"
// #include "groupConnectors.h"

using namespace std;

CommandTree* parse(string userInput) {
	// 1. Split on spaces
	vector<string> words = splitSpaces(userInput);

	// 2. Filter out #
	words = filterComments(words);

	// // 3. Group words into Tokens of type Subcommand or Operator.
	// vector<Token*> tokens;
	// tokens = tokenize(words);

	// // 4. Create a tree for execution in later steps
	CommandTree* ctree;
	// commands = constructExecutionTree(words);

	return ctree;
}
