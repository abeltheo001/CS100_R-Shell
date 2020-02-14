#include <iostream>
#include "parser.h"
// #include "../header/initConfig.h"
#include "executor.h"

using namespace std;

int main(int argc, char *argv[]) {
	// unordered_map<string, bool> config = initConfig("../.rshellrc");

	if (argc == 1) {
			string currInput = "";
			while (currInput != "exit") {
				cout << "$ ";
				getline(cin, currInput);		
				CommandTree ctree = parse(currInput);
				int returnval = executor(ctree);
				// cout << returnval << endl; // Debug
			}
	} else {
		vector<string> args(argv, argv+argc);
		args.erase(args.begin()); // Unfortunate inefficiency...
		string cmd = joinVector(args, ' ');
		CommandTree ctree = parse(cmd);
		int returnval = executor(ctree);
	}


}
