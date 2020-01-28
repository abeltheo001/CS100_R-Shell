#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;

int main() {
    // Procedure:
    // Fork another process and get the pid number
    // Run sleep 1 on the other process
    // Waitpid for that process
    // Print. If it takes 1 second, then everything is working properly.
    
    pid_t pid = fork();
    // At this point, this code splits into two. 
    // In the parent process (the current one), pid > 0. In the child process, pid == 0.

    //Each time while pid >= 0, the code will take in a array of pointers that end with NULL
    //It will then be put into the execvp() function, where it will find a match and perform the commmand. 
    if (pid >= 0)
    {
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

    

}
