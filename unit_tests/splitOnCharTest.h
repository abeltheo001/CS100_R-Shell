#ifndef __SPLITONCHARTEST_H__
#define __SPLITONCHARTEST_H__

#include "gtest/gtest.h"
#include "../header/splitOnChar.h"
#include <vector>
#include <string>

TEST (splitOnChar, singleSpace) {
	vector<string> values;
	values.push_back("abc");
	values.push_back("123");
	vector<string> outputV = splitOnChar("abc 123", ' ');
	EXPECT_EQ(values, outputV);
}

TEST (splitOnChar, flankingSpaces) {
	vector<string> values;
	values.push_back("abc");
	values.push_back("123");
	vector<string> outputV = splitOnChar("              abc 123              ", ' ');
	EXPECT_EQ(values, outputV);
}

TEST (splitOnChar, multipleMiddleSpaces) {
	vector<string> values;
	values.push_back("abc");
	values.push_back("123");
	vector<string> outputV = splitOnChar("abc             123", ' ');
	EXPECT_EQ(values, outputV);
}

TEST (splitOnChar, allSpaces) {
	vector<string> values;
	values.push_back("abc");
	values.push_back("123");
	vector<string> outputV = splitOnChar("             abc               123               ", ' ');
	EXPECT_EQ(values, outputV);
}

#endif
