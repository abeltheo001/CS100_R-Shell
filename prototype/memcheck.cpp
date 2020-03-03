#include <iostream>
#include <vector>

using namespace std;

class A {
	public:
		A() { }
		A(vector<string> V) : content(V) { }

		virtual int retInt() { return 1; }

		vector<string> content = {};
		A* leftChild = nullptr;
};

class B : public A {
	public:
		B() { }
		B(vector<string> V) { content = V; }

		virtual int retInt() { return 2; }
		virtual bool someBfunction() { return true; }

		int Bmembervar = 0;
};

vector<A*> myFunc(vector<vector<string>> things_to_make) {
	vector<A*> outputV = {};
	int vlen = 0;
	for (vector<string> V : things_to_make) {
		B* myB = new B(V);
		outputV.push_back(myB);
		if (vlen > 0) {
			myB->leftChild = outputV[vlen];
		}
		vlen++;
	}
	return outputV;
}

int main() {
	vector<vector<string>> things = {{"ls", "-al"}, {"echo", "a"}};
	vector<A*> myAs = myFunc(things);

	for (A* Aptr : myAs) {
		delete Aptr;
	}

	return 0;
}
