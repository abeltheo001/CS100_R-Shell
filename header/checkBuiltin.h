#ifndef CHECK_BUILTIN_H
#define CHECK_BUILTIN_H 

#include <iostream>
#include <string>
#include <vector>
#include "rshellclasses.h"
#include <algorithm>

using namespace std;

static const vector <string> operations {"cd","echo","mv","ls","pwd","mkdir","cp","kill","exit", "alias","pwd","exit","history","cat","chmod"};

bool checkBuiltin(Subcommand input)
{
	vector<string> values = input->content();
	if (find(operations.begin(),operations.end(),values[0])
		return true;

	return false; 
	
}


#endif
