#include <iostream>
#include "../header/rshellclasses.h"
#include "../header/rshelldefinition.h"
using namespace std;

int main(int argc, char *argv[]) {
	// unordered_map<string, bool> config = initConfig("../.rshellrc");
	
	RShell shellobj = RShell();

	if (argc == 1) {
			string currInput = "";
			while (currInput != "exit") {
				cout << "$ ";
				getline(cin, currInput);	
				shellobj.makeCommandDeque(currInput);
				shellobj.executeCommandDeque();
				if (GLOBAL_EXIT_STATUS == 1) {
					exit(0);
				}
				// cout << returnval << endl; // Debug
			}
	} else {
		// TODO: Re-add argv support
		// vector<string> args(argv, argv+argc);
		// args.erase(args.begin()); // Unfortunate inefficiency...
		// string cmd = joinVector(args, ' ');
		// shellobj.makeCommandTree(cmd);
		// shellobj.executeCommandTree();
	}
}
