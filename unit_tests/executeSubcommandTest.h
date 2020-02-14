#ifndef __EXECUTESUBCOMMAND_H__
#define __EXECUTESUBCOMMAND_H__

#include "gtest/gtest.h"
#include "../header/executeSubcommand.h"
#include <vector>
#include <string>

TEST (executeSubcommandTest, validCommand)
{
	vector <string> values = {"echo", "Hello", "World!"};

	EXPECT_EQ(0, executeSubcommand(values)); // Exit status for valid echo command
}

TEST (executeSubcommandTest, invalidCommand)
{
	vector<string> input = {"eabmoinmeoaimvpoapom3vap"};
    
    int returnval = executeSubcommand(input);
    
	EXPECT_EQ(-1, returnval); // Command doesn't exist
}

TEST (executeSubcommandTest, validButFails)
{
	vector <string> input;
	input.push_back("ls");
	input.push_back("-j");
	
	EXPECT_EQ(2, executeSubcommand(input)); // Exit status for ls -j
}


#endif
