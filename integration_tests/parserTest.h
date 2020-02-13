#ifndef __PARSERTEST_H__
#define __PARSERTEST_H__

#include "gtest/gtest.h"
#include "../header/tokenize.h"
#include "../src/parser.h"

TEST (parserTest, twoTallTree) {
	// For convenience's sake, I'm going to use tokenize()
	vector<string> values = {"ls", "-al", "||", "echo", "hello", "&&", "echo", "world"};
    vector<Token*> tokens = tokenize(values);

	// Tree should look like the following:
	//                        &&
	//				      /        \
	//                  ||       echo world
	//				/        \
	//           ls -al    echo hello

    CommandTree ctree = parse("ls -al || echo hello && echo world #ioj5v31mv05813uv095u1m309vm513");
    CommandTree manualtree;
    manualtree.setHead(tokens[tokens.size()-2]);
    (tokens[tokens.size()-2])->rightChild = tokens[tokens.size()-1];
    (tokens[tokens.size()-2])->leftChild = tokens[1];
    (tokens[1])->rightChild = tokens[2];
    (tokens[1])->leftChild = tokens[0];

    cout << ctree.stringify();
    cout << manualtree.stringify();

    bool succeeded = (ctree == manualtree);

    EXPECT_EQ(succeeded, true);
}

#endif
