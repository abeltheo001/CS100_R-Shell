#include "../header/rshellclasses.h"
#include "../header/rshelldefinition.h"
using namespace std;

int main(int argc, char *argv[]) {
	// unordered_map<string, bool> config = initConfig("../.rshellrc");

	if (argc == 1) {
		RShell shellobj = RShell(false); // (usually) Run with debug when in rshell mode, turned off for submission

		string currInput = "";
		while (currInput != "exit") {
			cout << "$ ";
			getline(cin, currInput);	
			shellobj.makeCommandDeque(currInput);
			shellobj.executeCommandDeque();
			if (GLOBAL_EXIT_STATUS == 1) {
				exit(0);
			}
		}
	} else {
		// Run without debug when in argv mode
		RShell shellobj = RShell(false);

		vector<string> args(argv, argv+argc);
		args.erase(args.begin()); // Unfortunate inefficiency...
		string cmd = joinVector(args, ' ');
		shellobj.makeCommandDeque(cmd);
		shellobj.executeCommandDeque();
	}
}
