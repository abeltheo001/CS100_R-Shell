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

    if (pid == 0) {
        unsigned int microseconds = 100000;
        usleep(microseconds);
        cout << "Child process complete" << endl;
    } else if (pid > 0) {
        waitpid(0, nullptr, 0); // Try commenting and uncommenting this line to see how it affects the program.
        cout << "Parent process complete" << endl;
    }

}
