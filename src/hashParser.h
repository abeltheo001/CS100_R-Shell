#include <iostream>
#include <string>

using namespace std;

string numParser (string &input)
{
	size_t  hyphen = input.find('#');
	input = input.substr(0,hyphen);
	
	return input; 
}
