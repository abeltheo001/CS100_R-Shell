#ifndef RSHELLDEFINITION_H
#define RSHELLDEFINITION_H

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "rshellclasses.h"
#include "rshellutils.h"
#include "splitOnChar.h"
#include "shuntingExecute.h"
#include "shuntingYardConstruct.h"

using namespace std;

void RShell::makeCommandDeque(string userInput)
{
	deque<Token*> resd = shuntingYardConstruct(userInput);
	this->commandDeque = resd;
	if (DEBUG) {
		for (Token* t : this->commandDeque) {
			cout << t->stringify() << endl;
		}
	}
}

int RShell::executeCommandDeque()
{
	if (DEBUG) {
		cout << "EXECUTING:" << endl;
		cout << "{" << endl;
	}

	int execres = shuntingExecute(this->commandDeque);

	if (DEBUG) {
		cout << "}" << endl;

		cout << "Final execution status:" << endl;
		for (Token* t : this->commandDeque) {
			cout << t->status << " ";
		}
		cout << endl;
	}

	deconstructDeque();

	this->commandDeque.clear();

	return execres;
}

#endif
