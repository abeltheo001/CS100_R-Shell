#include <iostream>
#include "parser.h"
#include "executor.h"

using namespace std;

int main() {
        string currInput = "echo a || false && echo c";
        CommandTree ctree = parse(currInput);
        int returnval = executor(ctree);
        // cout << ctree.stringify();
        // cout << returnval << endl; // Debug
}
