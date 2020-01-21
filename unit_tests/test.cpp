#include "gtest/gtest.h"
#include "../src/splitSpaces.h"
// #include "../src/hashParser.h"
#include <vector>
#include <iostream>

// TEST (hashParseTest, FirstCheck)
// {
// 	vector <string> values;
// 	values.push_back("hamina");
// 	values.push_back("#");	
// 	EXPECT_EQ("hamina",hashParser(values));
// } 

TEST (splitSpaces, singleSpace) {
	vector<string> values;
	values.push_back("abc");
	values.push_back("123");
	vector<string> outputV = splitSpaces("abc 123");
	EXPECT_EQ(values, outputV);
}
	


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
