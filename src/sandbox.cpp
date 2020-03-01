#include <iostream>
#include <iostream>
#include <vector>
#include <string>
#include "../header/rshellclasses.h"
#include "../header/rshelldefinition.h"


using namespace std;

void shuntingTest()
{
	
}

int main() {
	
	string input = "echo a || (echo b && echo c)";
	Rshell shell = Rshell();
	
	shell.makeCommandTree(input);
	shell.executeCommandTree();

	
	return 0;
}
