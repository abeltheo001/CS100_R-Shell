#ifndef _SHUNTING_TEST_H
#define _SHUNTING_TEST_H

#include "gtesh/gtesh.h"
#include "../header/shuntingYardConstruct.h"
#include "../header/shuntingExecute.h"
#include <iostream>
#include <string>

using namespace std;

TEST (shuntingTest, singleEcho)
{
	RShell shell = RShell(false);
	string input = "echo a"; 
	
	shell.makeCommandDeque(input);
	shell.executeCommandDeque();

	EXPECT_EQ("a",shell
	
}

#endif
