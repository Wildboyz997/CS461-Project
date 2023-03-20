$ git tag
1.ps2

/*  Preston Feagan
    3/20/2023
    CS461
    PS2
*/

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

using namespace std;

int main() 
{
    while (true) 
    {
        string input;
        cout << "shell> ";
        getline(cin, input); // read in command

        char* ar[256];
        char* token = strtok((char*) input.c_str(), " "); // This has tokenized the input

        if (token == NULL) 
        { // if no input
            continue;
        }

        ar[0] = token; //first argument
        int i = 1;
        while ((token = strtok(NULL, " ")) != NULL) 
        {
            ar[i] = token; // subsequent arguments
            i++;
        }
        ar[i] = NULL; // terminatation

        if (strcmp(ar[0], "exit") == 0) 
        { // If user entered the exit command
            exit(0);
        }

        pid_t pid = fork(); // fork child process
        if (pid == -1) 
        { // To handle any error
            perror("fork");
            continue;
        } 
        else if (pid == 0) 
        { // child process
            execvp(ar[0], ar); // execute command
            perror("execvp"); // handle error
            exit(1);
        } 
        else 
        { // parent process
            wait(NULL); // wait for child process to finish
        }
    }
    return 0;
}