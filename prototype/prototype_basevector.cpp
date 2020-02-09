#include "../header/rshellclasses.h"
#include <vector>
#include <typeinfo>
#include <iostream>

using namespace std;

int main() {
    vector<Token*> myPointers;
    Subcommand s({"echo", "a"});
    Operator o({"||"});

    myPointers.push_back(&s);
    myPointers.push_back(&o);
    
    cout << typeid(myPointers[0]).name() << endl;
    cout << typeid(myPointers[1]).name() << endl;
    cout << typeid(&s).name() << endl;
    cout << typeid(&o).name() << endl;

    return 0;
};
