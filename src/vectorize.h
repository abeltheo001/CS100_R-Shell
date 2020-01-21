#include <iostream>
#include <vector>

using namespace std;

void printVect(vector<string>& V) {
	auto it = V.begin();
	while(it != V.end()) {
		cout << *it << " ";
		V++;
	}
	cout << endl;
}

vector<string>& vectorize(int numArgs, char** words, vector<string>& outputVect) {
	if (*words == nullptr) {
		return outputVect;
	} else {
		for (int i=0; i<numArgs; i++) {
			outputVect.push_back(string(words[i]));
		}
		return outputVect;
	}
}
