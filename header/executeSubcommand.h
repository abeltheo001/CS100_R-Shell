#ifndef EXECUTESUBCOMMAND_CPP
#define EXECUTESUBCOMMAND_CPP
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h> 
#include <cstring>

using namespace std;

int executeSubcommand(vector<string> input)
{
        vector <char*> V;
        for (vector <string>::iterator it = input.begin(); it!=input.end();++it)
                V.push_back(const_cast<char*>(it->c_str()));

        V.push_back(nullptr);
        char** results = &V[0];

        pid_t pid = fork();
        if (pid < 0)
        {
            cout << """*** ERROR: forking child process failed\n" << endl;
            exit(1);

        }
        if (pid == 0)
        {
            if (execvp(results[0],results) < 0)
                {
                    cout << "error";
                    exit(1);
                }

        }
        else 
        {
            int status;
            waitpid(pid, &status, 0);
            if (status > 0)
            {
                return -1;
            }
        }

        return 0;

}
#endif
