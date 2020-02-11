#include <iostream>
#include "../header/rshellclasses.h"
#include "../header/splitOnChar.h"
#include "../header/filterComments.h"
#include "../header/tokenize.h"
#include "../header/groupQuotes.h"

using namespace std;

CommandTree* parse(string userInput) {
	// 1. Split on spaces
	vector<string> words = splitOnChar(userInput, ' ');

	// 2. Make sure quotes get grouped together
	words = groupQuotes(words)

	// 3. Filter out #
	words = filterComments(words);

	// 4. Group words/quotes into Tokens of type Subcommand or Operator.
	vector<Token*> tokens = tokenize(words);

	// 5. Create a tree for execution in later steps
	CommandTree* ctree;
	// commands = constructExecutionTree(words);

	return ctree;
}
