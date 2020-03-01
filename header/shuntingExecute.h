#ifndef SHUNTINGEXECUTE_H
#define SHUNTINGEXECUTE_H

#include <stack>
#include <queue>
#include <vector>

#include "rshellclasses.h"
#include "rshelldefinition.h"

using namespace std;

int RShell::shuntingExecute(deque<Token*> V) {
	stack<Token*> toExecute;

	if (V.size() == 0) {
		return 0;
	} else if (V.size() == 1) {
		return V[0]->execute();
	} else {
		for (Token* t : V) {
			if (t->isOperator == false) {
				toExecute.push(t);
			} else {
				Token* r = toExecute.top();
				toExecute.pop();
				Token* l = toExecute.top();
				toExecute.pop();

				t->leftChild = l;
				t->rightChild = r;

				t->execute();

				vector<Token*> pointers = {l, r, t};
				for (Token* currToken : pointers) {
					StorageToken* sptr = dynamic_cast<StorageToken*>(currToken);
					if (sptr != nullptr) {
						delete sptr;
					}
				}

				// Need to re-add execution value
				StorageToken* s = new StorageToken(t->status);
				toExecute.push(s);
			}
		}
	}
	
	
}

#endif