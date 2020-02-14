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
	vector <string> input;
	input.push_back("tjijamoifecjoeamvoifjmaf");
	
	EXPECT_EQ(-1, executeSubcommand(input)); // Command doesn't exist
}

TEST (executeSubcommandTest, validButFails)
{
	vector <string> input;
	input.push_back("ls");
	input.push_back("-j");
	
	EXPECT_EQ(512, executeSubcommand(input)); // Exit status for ls-j
}


#endif
