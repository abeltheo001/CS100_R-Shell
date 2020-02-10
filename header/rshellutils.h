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

#endif
