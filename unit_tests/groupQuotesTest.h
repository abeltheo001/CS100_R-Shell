#ifndef __GROUPQUOTESTEST_H__
#define __GROUPQUOTESTEST_H__

#include "gtest/gtest.h"
#include "../header/groupQuotes.h"
#include <vector>
#include <string>

TEST (groupQuotes, noQuotes) {
	vector<string> values;
	values.push_back("abc");
	values.push_back("123");
	vector<string> outputV = groupQuotes(values);
	EXPECT_EQ(values, outputV);
}

TEST (groupQuotes, quotesHelloWorld) {
	vector<string> sp = splitSpaces("echo \"Hello world\"");
	vector<string> grouped = groupQuotes(sp);

	vector<string> expected = {"echo", "\"Hello world\""};
	EXPECT_EQ(grouped, expected);
}

TEST (groupQuotes, quotesHelloWorldFlanked) {
	vector<string> sp = splitSpaces("echo aaa\"Hello world\"aaa");
	vector<string> grouped = groupQuotes(sp);

	vector<string> expected = {"echo", "aaa\"Hello world\"aaa"};
	EXPECT_EQ(grouped, expected);
}

#endif
