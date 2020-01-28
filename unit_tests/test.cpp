#include "gtest/gtest.h"
#include "../header/splitSpaces.h"
#include "../header/filterComments.h"
#include <vector>
#include <iostream>

 TEST (hashParseTest, FirstCheck)
 {
 	vector <string> values;
 	values.push_back("hamina");
 	values.push_back("#");
	values.push_back("hello");
	vector<string> output;
	output.push_back("hamina");	
 	EXPECT_EQ(output, filterComments(values));
 } 

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


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
