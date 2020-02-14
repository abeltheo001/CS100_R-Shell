#ifndef EXECUTEVECTOR_H
#define EXECUTEVECTOR_H
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h> 
#include <cstring>

using namespace std;

int executeVector(vector<string> input)
{
        // Generate char pointer vector (needed for execvp)
        vector <char*> V;
        for (auto it = input.begin(); it!=input.end();++it)
                V.push_back(const_cast<char*>(it->c_str()));
        
        V.push_back(nullptr);   // execvp char pointer array must end with nullptr 
        char** results = &V[0]; // convert char pointer vector to char pointer array

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
            execvp(results[0], results);
            // If it returns, the results[0] is unknown
            return 2;
        }
        else 
        {
            // Parent process
            int status = -1;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) != 0) {
                return WEXITSTATUS(status);
            } else {
                return 3; // Called program segfaulted or something similar
            }
        }
}
#endif
