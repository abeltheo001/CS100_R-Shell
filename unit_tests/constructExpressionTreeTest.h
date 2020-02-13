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
    
    cout << ctree.stringify();

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

	// Tree should look like the following:
	//                  ||
	//				/        \
	//           ls -al    echo hello

	vector<string> left = {"ls", "-al"};
	vector<string> right = {"echo", "hello"};
	vector<string> op = {"||"};
	Token* opObj = new Operator(op);
	Token* leftObj = new Subcommand(left);
	Token* rightObj = new Subcommand(right);

	EXPECT_EQ(*(ctree.getHead()), *opObj);
}

TEST (constructExpressionTreeTest, withBashConnectors_Left) {
	// For convenience's sake, I'm going to use tokenize()
	vector<string> values = {"ls", "-al", "||", "echo", "hello"};
	vector<Token*> tokens = tokenize(values);

	CommandTree ctree = constructExpressionTree(tokens);

	// Tree should look like the following:
	//                  ||
	//				/        \
	//           ls -al    echo hello

	vector<string> left = {"ls", "-al"};
	vector<string> right = {"echo", "hello"};
	vector<string> op = {"||"};
	Token* opObj = new Operator(op);
	Token* leftObj = new Subcommand(left);
	Token* rightObj = new Subcommand(right);
	
	EXPECT_EQ(*((ctree.getHead())->leftChild), *leftObj);
}

TEST (constructExpressionTreeTest, withBashConnectors_Right) {
	// For convenience's sake, I'm going to use tokenize()
	vector<string> values = {"ls", "-al", "||", "echo", "hello"};
	vector<Token*> tokens = tokenize(values);

	CommandTree ctree = constructExpressionTree(tokens);

	// Tree should look like the following:
	//                  ||
	//				/        \
	//           ls -al    echo hello

    cout << ctree.stringify();

	vector<string> left = {"ls", "-al"};
	vector<string> right = {"echo", "hello"};
	vector<string> op = {"||"};
	Token* opObj = new Operator(op);
	Token* leftObj = new Subcommand(left);
	Token* rightObj = new Subcommand(right);

	EXPECT_EQ(*((ctree.getHead())->rightChild), *rightObj);
}

TEST (constructExpressionTreeTest, twoTallTree) {
	// For convenience's sake, I'm going to use tokenize()
	vector<string> values = {"ls", "-al", "||", "echo", "hello", "&&", "echo", "world"};
	vector<Token*> tokens = tokenize(values);

	CommandTree ctree = constructExpressionTree(tokens);

	// Tree should look like the following:
	//                        &&
	//				      /        \
	//                  ||       echo world
	//				/        \
	//           ls -al    echo hello

	vector<string> leftBottom = {"ls", "-al"};
	vector<string> rightBottom = {"echo", "hello"};
	vector<string> opLeft = {"||"};
    vector<string> opTop = {"&&"};
    vector<string> rightTop = {"echo", "world"};
	Token* opLeftObj = new Operator(opLeft);
	Token* opTopObj = new Operator(opTop);
	Token* leftBottomObj = new Subcommand(leftBottom);
	Token* rightBottomObj = new Subcommand(rightBottom);
	Token* rightTopObj = new Subcommand(rightTop);
  
    cout << ctree.stringify();

    bool succeeded = true;

    Token* t = ctree.getHead();
    if (!(*t == *opTopObj)) {
        succeeded = false;
    }
    if (!(*(t->rightChild) == *rightTopObj)) {
        succeeded = false;
    }
    
    Token* left_t = t->leftChild;
    if (!(*left_t == *opLeftObj)) {
        succeeded = false;
    }
    if (!(*(left_t->leftChild) == *leftBottomObj)) {
        succeeded = false;
    }
    if (!(*(left_t->rightChild) == *rightBottomObj)) {
        succeeded = false;
    }

    EXPECT_EQ(succeeded, false);
}

#endif
