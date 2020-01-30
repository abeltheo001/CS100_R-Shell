#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;

int main() {
    // The code will take in a array of pointers that end with NULL
    // It will then be put into the execvp() function, where it will find a match and perform the commmand. 
    char *argv[] = {"echo", "Hello", "World!", NULL};

    if (argv[3] == NULL)
    {
        execvp("echo", argv);
    }
    else
    {
        return -1;
    }

}
