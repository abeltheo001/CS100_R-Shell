#include <iostream>
#include "parser.h"
#include "../header/initConfig.h"

using namespace std;

int main() {
	unordered_map<string, bool> config = initConfig("../.rshellrc");

	string currInput = "";
	while (currInput != "exit") {
		cout << "$ ";
		getline(cin, currInput);
		
	}
}
