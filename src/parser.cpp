#include <iostream>
#include "tokenClasses.h"
#include "splitSpaces.h"
#include "hashParser.h" // TODO: Rename
#include "groupQuotes.h"
#include "groupConnectors.h"

using namespace std;

MultiToken* parse(string userInput) {
	// 1. Split on spaces
	vector<Token>& words;
	splitSpaces(userInput, words);
	// 2. Filter out #
	hashParser(words); // TODO: Rename to removeComment
	// 3. Group quotes into Tokens
	words = groupQuotes(words);
	// 4. Group bash connectors into MultiTokens
	vector<MultiToken>& commands;
	commands = groupConnectors(words);

	// Do stuff with the parsed input
}
