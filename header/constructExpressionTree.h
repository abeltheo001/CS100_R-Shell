#ifndef CONSTRUCTEXPRESSIONTREE_H
#define CONSTRUCTEXPRESSIONTREE_H

// https://stackoverflow.com/questions/29046384/finding-preoder-from-just-inorder-traversal/39707071

// If I want a balanced tree, start in the middle. Otherwise (like in this case), probably use left precedence.
// Unfortunately, this is probably going to mean a rewrite of the parser when we start using parentheses.
// Parentheses are split into two characters and hold a higher priority than the other bash operators.

#include <iostream>
#include <vector>
#include "rshellclasses.h"
#include "rshellutils.h"

using namespace std;

void Rshell::constructSubTree(const vector<Token*>& allNodes, int currPos) {
    // Constructs a three subgroup, then recurses
    //         ...
    //       B
    //     A   C
    // ...  ...
    // currPos refers to C in the three subgroup
	if (currPos == 2) { 
		allNodes[currPos-1]->rightChild = allNodes[currPos];
		allNodes[currPos-1]->leftChild = allNodes[currPos-2];
		return;
	} else {
        // allNodes[currPos-1] is the operator
		allNodes[currPos-1]->rightChild = allNodes[currPos];
		allNodes[currPos-1]->leftChild = allNodes[currPos-3];
		constructSubTree(allNodes, currPos-2);
	}
}

CommandTree constructExpressionTree(vector<Token*> objs) {
	CommandTree tree;

	if (objs.size() != 0) {
			if (objs.size() == 1) {	 		// Trivial case
				tree.setHead(objs[0]);
			} else {						// Constructed directly from inorder using left precedence
				tree.setHead(objs[objs.size()-2]);
				constructSubTree(objs, objs.size()-1);
			}
	}

	return tree;
}

#endif
