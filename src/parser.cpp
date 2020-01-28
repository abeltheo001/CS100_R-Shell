#include <iostream>
#include "../header/rshellclasses.h"
#include "../header/splitSpaces.h"
#include "../header/filterComments.h" // TODO: Rename
// #include "groupQuotes.h"
// #include "groupConnectors.h"

using namespace std;

CommandTree* parse(string userInput) {
	// 1. Split on spaces
	vector<string> words = splitSpaces(userInput);

	// 2. Filter out #
	words = filterComments(words);

	// // 3. Group quotes into Tokens
	// vector<Token*> tokens;
	// tokens = tokenize(words);

	// // 4. Group bash connectors into MultiTokens
	// vector<MultiToken>& commands;
	CommandTree* ctree;
	// commands = groupConnectors(words);

	return ctree;
}
