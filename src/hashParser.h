#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector <string> hashParser (vector <string> input)
{
        vector <string> result;
        for (int i = 0; input[i] != "#"; i++)
        {
                result.push_back(input[i]);     
                cout << result[i] << " ";   
        }
        cout << endl;

        return result; 
}

