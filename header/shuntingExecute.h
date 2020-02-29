#include <stack>
#include <queue>
#include <vector>

#include "rshellclasses.h"
#include "rshelldefinition.h"

using namespace std;

int shuntingExecute(vector<Token*> V) {
	stack<Token*> toExecute;
	
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
		}
	}
}
