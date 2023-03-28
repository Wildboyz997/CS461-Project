/*  Preston Feagan
    3/28/2023
    CS461
    PS3
*/

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h> // for file I/O

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
        bool redirect_input = false;
        bool redirect_output = false;
        int input_fd, output_fd;
        while ((token = strtok(NULL, " ")) != NULL) 
        {
            if (strcmp(token, "<") == 0) 
            { // redirect input
                redirect_input = true;
                token = strtok(NULL, " "); // get input file name
                input_fd = open(token, O_RDONLY); // open input file
                if (input_fd == -1) 
                {
                    perror("open");
                    continue;
                }
                dup2(input_fd, STDIN_FILENO); // redirect input to file
                close(input_fd); // close input file
                continue;
            } 
            else if (strcmp(token, ">") == 0) 
            { // redirect output
                redirect_output = true;
                token = strtok(NULL, " "); // get output file name
                output_fd = open(token, O_CREAT | O_WRONLY | O_TRUNC, 0644); // open output file
                if (output_fd == -1) 
                {
                    perror("open");
                    continue;
                }
                dup2(output_fd, STDOUT_FILENO); // redirect output to file
                close(output_fd); // close output file
                continue;
            }
            ar[i] = token; // subsequent arguments
            i++;
        }
        ar[i] = NULL; // terminatation

        if (strcmp(ar[0], "exit") == 0) 
        { // If user entered the exit command
            exit(0);
        }

        pid_t pid = fork(); // fork child process
        if (pid == -1) { // To handle any error
            perror("fork");
            continue;
        } 
        else if (pid == 0) 
        { // child process
            execvp(ar[0], ar); // execute command
            perror("execvp"); // handle error
            exit(1);
        } else 
        { // parent process
            wait(NULL); // wait for child process to finish
            if (redirect_input) 
            {
                dup2(STDIN_FILENO, input_fd); // restore original input
            }
            if (redirect_output) 
            {
                dup2(STDOUT_FILENO, output_fd); // restore original output
            }
        }
    }
    return 0;
}