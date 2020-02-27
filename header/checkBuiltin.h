#ifndef CHECK_BUILTIN_H
#define CHECK_BUILTIN_H 

#include <string>
#include <vector>
#include "rshellclasses.h"
#include <unordered_set>
using namespace std;

//static const vector <string> operations {"cd","echo","mv","ls","pwd","mkdir","cp","kill","exit", "alias","pwd","exit","history","cat","chmod"};

bool RShell::checkBuiltin(vector<string> input)
{
	unordered_set<string> operations = {"exit"};
	return (operations.count(input[0]) > 0);
}


#endif
