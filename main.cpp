#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char *argv[])
{
    int option = 0; // default option: execute the command ls -l and terminate normally

    // Get flag from command line
    int opt;
    while ((opt = getopt(argc, argv, "n:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            option = atoi(optarg);
            break;
        }
    }

    pid_t child_pid;

    /* FORK A NEW PROCESS */
    child_pid = fork();

    if (child_pid == -1)
    {
        cout << "Fork failed" << endl;
        return 1;
    }
    else if (child_pid == 0)
    {
        cout << "Hello from the child process!" << endl;
        cout << "The parent process ID is " << getppid() << endl;

        if (option % 2 == 0) // if the option number is even, execute the command ls -l and terminate normally
        {
            std::cout << "The child process will execute the command: ls -l after 6 seconds" << std::endl;
            sleep(6); /* SLEEP FOR 6 SECONDS*/
            /* EXECUTE THE COMMAND ls -l USING EXECVP*/
            char *cmd[] = {(char *)"ls", (char *)"-l", NULL};
            execvp(cmd[0], cmd);
        }
        else // if the option number is odd, terminate with a kill signal
        {
            std::cout << "The child process is exiting" << std::endl;
            kill(getpid(), SIGINT);
        }
    }
    else if (child_pid == 1)
    {
        int status;

        /* WAIT FOR CHILD PROCESS TO FINISH */
        waitpid(child_pid, &status, 0);

        cout << "\nHello from the parent process!" << endl;

        /* PRINT THE CHILD PID value: "The child process ID is $ID" */
        cout << "The child process ID is " << child_pid << endl;

        /* TODO: PRINT THE EXIT STATUS OF THE CHILD PROCESS BASED waitpid().
        MAKE SURE TO PASS BY REFERENCE THE STATUS VARIABLE TO THE SECOND PARAMETER OF waitpid()
        IF WIFEXITED, PRINT THE MESSAGE "The child process exited normally" WITH ENDLINE
        IF WIFSIGNALED, PRINT THE MESSAGE "The child process exited due to the kill signal" WITH ENDLINE
        */
        if (WIFEXITED(status))
        {
            cout << "The child process exited normally" << endl;
        }
        else if (WIFSIGNALED(status))
        {
            cout << "The child process exited due to the kill signal" << endl;
        }
    }

    return 0;
}
