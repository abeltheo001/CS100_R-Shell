#ifndef __CHECKBUILTIN_H__
#define __CHECKBUILTIN_H__

#include "gtest/gtest.h"
#include "../header/checkBuiltin.h"
#include <vector>
#include <string>

TEST (builtinTest, nonNULLending)
{
	vector <string> values;
	values.push_back("echo");

	EXPECT_EQ(false, checkBuiltin(values));
}

TEST (builtinTest, NULLending)
{
	vector <string> input;
	input.push_back("exit");
	
	EXPECT_EQ(true, checkBuiltin(input));	
}


#endif
