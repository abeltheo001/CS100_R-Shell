#include <iostream>
#include "parser.h"
#include "executor.h"

using namespace std;

int main() {
    CommandTree ct;
    
	vector<string> left = {"ls", "-al"};
	// vector<string> right = {"echo", "hello"};
	// vector<string> op = {"||"};
	// Operator opObj = Operator(op);
	Token* leftObj = new Subcommand(left);
	// Subcommand rightObj = Subcommand(right);

    ct.setHead(leftObj);

    int retval = executor(ct);

    delete leftObj;

    return 0;
}
