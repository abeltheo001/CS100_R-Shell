#ifndef __FILTERCOMMENTS_H__
#define __FILTERCOMMENTS_H__

#include "gtest/gtest.h"
#include "../header/filterComments.h"
#include <vector>
#include <string>

TEST (filterCommentsTest, FirstCheck)
{
    vector <string> values;
    values.push_back("hamina");
    values.push_back("#");
    values.push_back("hello");
    vector<string> output;
    output.push_back("hamina");	
    EXPECT_EQ(output, filterComments(values));
}

TEST (filterCommentsTest, LetterthenComment)
{
	vector <string> values = {"echo","a","#b","c","d"};
	vector <string> output = {"echo","a"};
	EXPECT_EQ(output,filterComments(values));
}

TEST (filterCommentsTest, CommentthenLetter)
{
	vector <string> values = {"echo", "1", "2", "3#"};
	EXPECT_EQ(values, filterComments(values));
}
#endif
