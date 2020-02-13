#ifndef __EXECUTESUBCOMMAND_H__
#define __EXECUTESUBCOMMAND_H__

#include "gtest/gtest.h"
#include "../header/executeSubcommand.h"
#include <vector>
#include <string>

TEST (executeSubcommandTest, NULLending)
{
	vector <string> values;
	values.push_back("echo");
	values.push_back("Hello");
	values.push_back("World!");

	EXPECT_EQ(0, executeSubcommand(values));
}

TEST (executeSubcommandTest, NoNULLending)
{
	vector <string> input;
	input.push_back("ls");
	input.push_back("-j");
	
	EXPECT_EQ(-1, executeSubcommand(input));	
}


#endif
