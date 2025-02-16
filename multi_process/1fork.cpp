#include<stdio.h>
#include<sys/types.h>//sys/types.h is a header file in the C Standard Library introduced for the Unix operating system that defines data types used in system calls. These types are used in system calls and library functions that deal with system calls.
#include<unistd.h> //unistd.h is a header file in the C Standard Library for the C programming language that defines symbolic constants and types found in the POSIX operating system API. These are used in the C library functions that deal with system calls.

int main()
{
    pid_t pid;
    pid = fork();
    if(pid == 0)
    {
        //child process
        printf("pid is %d Child process is running\n", pid);
    }
    else if(pid > 0)
    {
        //parent process
        printf("pid is %d Parent process is running\n",pid);
    }
    else
    {
        printf("Fork failed\n");
    }
    return 0;
}