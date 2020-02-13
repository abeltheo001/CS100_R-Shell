#ifndef RSHELLUTILS_H
#define RSHELLUTILS_H

#include <iostream>
#include <vector>

template <class T>
void printVector(vector<T> V, string delimiter=" ") {
	auto it = V.begin();
	while (it != V.end()) {
		cout << *it << delimiter;
		it++;
	}
}

string joinVector(vector<string> V, char joiner) {
	string concat;
	bool first = true;
	for (const string& substr : V) {
		if (first) {
			concat += substr;
			first = false;
		} else {
			concat += joiner;
			concat += substr;
		}
	}
	return concat;
}

string joinVector(vector<string> V) {
	string concat;
	bool first = true;
	for (const string& substr : V) {
		if (first) {
			concat += substr;
			first = false;
		} else {
			concat += substr;
		}
	}
	return concat;
}

#endif
