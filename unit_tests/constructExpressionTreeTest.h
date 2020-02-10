#ifndef __CONSTRUCTEXPRESSIONTREETEST_H__
#define __CONSTRUCTEXPRESSIONTREETEST_H__

#include "gtest/gtest.h"
#include "../header/rshellclasses.h"
#include "../header/tokenize.h"
#include "../header/constructExpressionTree.h"
#include <vector>
#include <string>

TEST (constructExpressionTreeTest, noBashConnectors) {
	// For convenience's sake, I'm going to use tokenize()
	vector<string> values;
	values.push_back("ls");
	values.push_back("-al");
	vector<Token*> tokens = tokenize(values);

	CommandTree ctree = constructExpressionTree(tokens);

	bool testsuccess = true;

	if ((ctree.getHead())->hasChildren()) {
		testsuccess = false;
	}

	EXPECT_EQ(testsuccess, true);
}

TEST (constructExpressionTreeTest, withBashConnectors_Head) {
	// For convenience's sake, I'm going to use tokenize()
	vector<string> values = {"ls", "-al", "||", "echo", "hello"};
	vector<Token*> tokens = tokenize(values);

	CommandTree ctree = constructExpressionTree(tokens);

	bool testsuccess = true;

	// Tree should look like the following:
	//                  ||
	//				/        \
	//           ls -al    echo hello

	vector<string> left = {"ls", "-al"};
	vector<string> right = {"echo", "hello"};
	vector<string> op = {"||"};

	EXPECT_EQ(*(ctree.getHead()), op);
}

TEST (constructExpressionTreeTest, withBashConnectors_Left) {
	// For convenience's sake, I'm going to use tokenize()
	vector<string> values = {"ls", "-al", "||", "echo", "hello"};
	vector<Token*> tokens = tokenize(values);

	CommandTree ctree = constructExpressionTree(tokens);

	bool testsuccess = true;

	// Tree should look like the following:
	//                  ||
	//				/        \
	//           ls -al    echo hello

	vector<string> left = {"ls", "-al"};
	vector<string> right = {"echo", "hello"};
	vector<string> op = {"||"};

	EXPECT_EQ(*((ctree.getHead())->leftChild), left);
}

TEST (constructExpressionTreeTest, withBashConnectors_Right) {
	// For convenience's sake, I'm going to use tokenize()
	vector<string> values = {"ls", "-al", "||", "echo", "hello"};
	vector<Token*> tokens = tokenize(values);

	CommandTree ctree = constructExpressionTree(tokens);

	bool testsuccess = true;

	// Tree should look like the following:
	//                  ||
	//				/        \
	//           ls -al    echo hello

	vector<string> left = {"ls", "-al"};
	vector<string> right = {"echo", "hello"};
	vector<string> op = {"||"};

	EXPECT_EQ(*((ctree.getHead())->leftChild), right);
}

#endif
