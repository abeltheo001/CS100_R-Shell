#ifndef __CONVERTVECTORTOCHAR__
#define __CONVERTVECTORTOCHAR__

#include <string>
#include <iostream>
#include <vector>

using namespace std;

char** convertVectorToCharArray(vector<string> Vin) {
    // Generate char pointer vector (needed for execvp)
    char** arr = new char*[Vin.size()+1];
    for (int i = 0 ; i < Vin.size(); i++) {
        arr[i] = new char[Vin[i].size() + 1];
        const char* cstr = Vin[i].c_str(); // Managed by the Vin[i] obj https://stackoverflow.com/questions/8843604/string-c-str-deallocation-necessary
                                           // Hence no need to delete.
        strcpy(arr[i], cstr);
    }
    
    arr[Vin.size()] = nullptr;   // execvp char pointer array must end with nullptr 

    return arr;
}

#endif
