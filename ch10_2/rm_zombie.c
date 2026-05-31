#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include <sys/wait.h>

void read_childproc(int sig)
{
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status))
    {
        printf("Remove porc id: %d\n", pid);
        printf("Child proc: %d", WEXITSTATUS(status));
    }
}

int main(int argc, char* argv[])
{
    pid_t pid;
    struct sigaction act;

    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);


    pid = fork();

    if(pid == 0)
    {
        puts("Hi I'm Childproc!");
        sleep(30);
        return 12;
    }
    else
    {
        puts("Hi! I'm ParentsProc");
        pid = fork();
        if(pid == 0)
        {
            puts("Hi I'm ChildProc2");
            sleep(10);
            return 24;
        }
        else{
            printf("Child proc id: %d", pid);
            for(int i = 0; i < 5; i++)
            {
                puts("wait...");
                sleep(5);
            }
        }
    }
    printf("SigChld: %d", SIGCHLD);
    return 0;
}