#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void read_childporc(int sig)
{
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status))
    {
        printf("Removed proc ID: %d\n", pid);
        printf("Child send: %d\n", WEXITSTATUS(status));
    }
}

int main(int argc, char *argv[])
{
    pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childporc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    pid = fork();
    if(pid == 0)
    {
        puts("Hi Im Child1");
        sleep(10);
        return 12;
    }
    else
    {
        printf("Hi Im Parents\n");
        pid = fork();
        if (pid == 0)
        {
            puts("Hi Im Child2");
            sleep(15);
            exit(24);
        }
        else
        {
            int i;
            printf("Child Proc id: %d\n", pid);
            for(i = 0; i < 5; i++)
            {
                puts("wait...");
                sleep(5);
            }
        }
    }
    printf("SigChild: %d\n", SIGCHLD);
    return 0;
}