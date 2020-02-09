#ifndef __FILTERCOMMENTS_H__
#define __FILTERCOMMENTS_H__

#include "gtest/gtest.h"
#include "../header/filterComments.h"
#include <vector>
#include <string>

TEST (hashParseTest, FirstCheck)
{
    vector <string> values;
    values.push_back("hamina");
    values.push_back("#");
    values.push_back("hello");
    vector<string> output;
    output.push_back("hamina");	
    EXPECT_EQ(output, filterComments(values));
}

#endif