#ifndef __TOKENIZETEST_H__
#define __TOKENIZETEST_H__

#include "gtest/gtest.h"
#include "../header/rshellclasses.h"
#include "../header/tokenize.h"
#include <vector>
#include <string>

TEST (tokenizeTest, withOperator) {
	vector<string> values = {"ls", "-al", "||", "echo", "a"};
	vector<Token*> outputV = tokenize(values);
	
	Subcommand lsal = Subcommand({"ls", "-al"});
	Operator op = Operator({"||"});
	Subcommand echoa = Subcommand({"echo", "a"});
	vector<Token*> expected = {&lsal, &op, &echoa};

	bool equal = true;
	for (int i = 0; i < outputV.size(); i++) {
		if (!( (*outputV[i]) == (*expected[i]) ) ) {
			equal = false;
			break;
		}
	}

	EXPECT_EQ(equal, true);
}

TEST (tokenizeTest, withoutOperator) {
	vector<string> values;
	values.push_back("ls");
	values.push_back("-al");
	vector<Token*> outputV = tokenize(values);
	
	Subcommand a = Subcommand({"ls", "-al"});

	bool equal = true;
	for (int i = 0; i < outputV.size(); i++) {
		if (!( (*outputV[i]) == a ) ) {
			equal = false;
			break;
		}
	}

	EXPECT_EQ(equal, true);
}
#endif
