#include <iostream>
#include <vector>

using namespace std;

class A {
    public:
        A() { }
        A(vector<string> V) : content(V) { }
        vector<string> content = {};
};

class B : public A {
    public:
        B() { }
        B(vector<string> V) { content = V; }
        bool specialVFunction() { return true; }
};

// Hypothesis: copying a vector might delete heap memory inside

vector<A*> myFunc(vector<vector<string>> things_to_make) {
    vector<A*> outputV = {};
    for (vector<string> V : things_to_make) {
        B* myB = new B(V);
        outputV.push_back(myB);
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
