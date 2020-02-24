#ifndef GROUPQUOTES_H
#define GROUPQUOTES_H

#include <iostream>
#include <vector>
#include "rshellutils.h"
#include <cstdio>
#include "rshellclasses.h"
// Input here is {"echo", "\"Hello", "World\""}. Want it to instead be {"echo", ""Hello world""}.
// Edge case: echo "Hello || world"aaa prints "Hello || worldaaa". I suspect this means the execvp command
//     was fed {"echo", ""Hello || world"aaa"}. As a result, quotes may be inside preexisting strings instead
//	   of just at the start. splitOnChar would turn that statement into {"echo", "\"Hello", "||", "world\"aaa"} already,
//	   so fortunately it's just a matter of merging strings between the open quote and close quote.

using namespace std;

vector<string> Rshell::groupQuotes(vector<string> Vin) {
	vector<string> grouped;
	vector<string> buffer;	
	bool state = 0; // 0 = normal mode. passes words from Vin to grouped
					// 1 = quote mode. passes words to buffer instead. when endquote reached,
					//     dump buffer into a string and append to grouped.

	auto it = Vin.begin();
	while (it != Vin.end()) { // Iterate over Vin
		bool quotefound = false;
		for (char c : (*it)) { // Iterate over substring
			if (c == '"') {
				quotefound = true;
				break;
			}
		}
		if (quotefound) {
			if (state) { 	// Endquote reached, concatenate buffer and dump to grouped 
				// See https://stackoverflow.com/questions/15347123/how-to-construct-a-stdstring-from-a-stdvectorstring/18703743
				//     for some discussion about concatting strings in C++.
				state = 0;
				buffer.push_back(*it);
				string concat = joinVector(buffer, ' ');
				grouped.push_back(concat);
				buffer.clear();
			} else { 		// Startquote reached, switch state
				state = 1;
				buffer.push_back(*it);
			}
		} else {
			if (state) { 	// In quote mode, so dump to buffer
				buffer.push_back(*it);
			} else { 		// In normal mode, so dump to grouped
				grouped.push_back(*it);
			}
		}
		it++;
	}

	// If there's still stuff on the buffer, something is wrong
	if (buffer.size() > 0) {
        string s = "No end quote in quote grouping.";
        const char* errormsg = s.c_str();
        perror(errormsg);
	}

	return grouped;
}

#endif
