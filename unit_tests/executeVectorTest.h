#ifndef __EXECUTEVECTORTEST_H__
#define __EXECUTEVECTORTEST_H__

#include "gtest/gtest.h"
#include "../header/executeVector.h"
#include <vector>
#include <string>

TEST (executeVectorTest, validCommand)
{
	vector <string> values = {"echo", "Hello", "World!"};

	EXPECT_EQ(0, executeVector(values)); // Exit status for valid echo command
}

TEST (executeVectorTest, invalidCommand)
{
	vector<string> input = {"eabmoinmeoaimvpoapom3vap"};
    
    int returnval = executeVector(input);
    
	EXPECT_EQ(2, returnval); // Command doesn't exist
}

TEST (executeVectorTest, validButFails)
{
	vector <string> input;
	input.push_back("ls");
	input.push_back("-j");
	
	EXPECT_EQ(2, executeVector(input)); // Exit status for ls -j
}


#endif
