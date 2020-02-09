#ifndef __SPLITSPACES_H__
#define __SPLITSPACES_H__

#include "gtest/gtest.h"
#include "../header/splitSpaces.h"
#include <vector>
#include <string>

TEST (splitSpaces, singleSpace) {
	vector<string> values;
	values.push_back("abc");
	values.push_back("123");
	vector<string> outputV = splitSpaces("abc 123");
	EXPECT_EQ(values, outputV);
}

TEST (splitSpaces, flankingSpaces) {
	vector<string> values;
	values.push_back("abc");
	values.push_back("123");
	vector<string> outputV = splitSpaces("              abc 123              ");
	EXPECT_EQ(values, outputV);
}

TEST (splitSpaces, multipleMiddleSpaces) {
	vector<string> values;
	values.push_back("abc");
	values.push_back("123");
	vector<string> outputV = splitSpaces("abc             123");
	EXPECT_EQ(values, outputV);
}

TEST (splitSpaces, allSpaces) {
	vector<string> values;
	values.push_back("abc");
	values.push_back("123");
	vector<string> outputV = splitSpaces("             abc               123               ");
	EXPECT_EQ(values, outputV);
}

#endif