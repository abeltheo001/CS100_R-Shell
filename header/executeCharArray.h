#ifndef EXECUTECHARARRAY_H
#define EXECUTECHARARRAY_H
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h> 
#include <cstring>

using namespace std;

int executeCharArray(char** charIn)
{
        pid_t pid = fork();
        if (pid < 0)
        {
            string s = "ERROR: forking child process failed.";
            const char* errormsg = s.c_str();
            perror(errormsg);
        }
        else if (pid == 0)
        {
            // Child process
            // Akin to *argv[0], the first argument is the name of the thing it's being called inside
            execvp(charIn[0], charIn);
            // If it returns, the charIn[0] is unknown
            // return -1; // -1 as an exit status has undefined behavior https://stackoverflow.com/questions/18890534/c-how-can-i-return-a-negative-value-in-main-cpp
            return 47;
        }
        else 
        {
            // Parent process
            int status = 0;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) != 0) {
                return WEXITSTATUS(status);
            } else {
                return -3; // Called program segfaulted or something similar
            }
        }
}
#endif
