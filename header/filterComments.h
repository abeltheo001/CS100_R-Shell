#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector <string> filterComments (vector <string> input)
{
        vector <string> result;
        for (vector<string>::iterator it = input.begin(); it != input.end(); ++it)
        {
		string check = *it;
		if (check[0] == '#')
		{
			break;
		}
		else
		{
			result.push_back(check);
		}
        }

        return result; 
}

