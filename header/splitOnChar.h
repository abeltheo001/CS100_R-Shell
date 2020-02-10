#ifndef SPLITONCHAR_H
#define SPLITONCHAR_H

#include <iostream>
#include <vector>

using namespace std;

vector<string> splitOnChar(string userInput, char sp) {
	vector<char> buffer;
	auto it = userInput.begin();
	vector<string> outputV;
	while (it != userInput.end()) {
		char currChar = *it;
		if (currChar != sp) {
			buffer.push_back(currChar);
		} else {
			if (buffer.size() > 0) {
				string s(buffer.begin(), buffer.end());
				outputV.push_back(s);
				buffer.clear();
			}
		}
		it++;
	}
	// Remove any remaining stuff on the buffer
	if (buffer.size() > 0) {
		string s(buffer.begin(), buffer.end());
		outputV.push_back(s);
	}
	return outputV;
}

#endif
