#include <iostream>
#include "parser.h"
#include "executor.h"

using namespace std;

int main() {
        string userInput = "echo a || false && echo c";
        
        vector<string> test = splitOnChar(userInput,';');
        userInput = joinVector(test," ;"); 

        // 1. Split on spaces
        vector<string> words = splitOnChar(userInput, ' ');

        // 2. Make sure quotes get grouped together
        words = groupQuotes(words);

        // 3. Filter out #
        words = filterComments(words);

        // 4. Group words/quotes into Tokens of type Subcommand or Operator.
        vector<Token*> tokens = tokenize(words);
       
        for (Token* t : tokens) delete t;

        // CommandTree ctree = parse(currInput);

        // cout << ctree.stringify();
        // cout << returnval << endl; // Debug
        return 0;
}
