#include "gtest/gtest.h"

TEST (hashParseTest, FirstCheck)
{
	vector <string> values;
	values.push_back("hamina");
	values.push_back("#");	
	EXPECT_EQ("hamina",hashParser(values));
} 

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
