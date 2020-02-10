#ifndef FILTERCOMMENTS_H
#define FILTERCOMMENTS_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector <string> filterComments (vector <string> input)
{
        vector <string> result;
        for (int i = 0; input[i] != "#"; i++)
        {
                result.push_back(input[i]);       
        }

        return result; 
}

#endif
