#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    pid_t pid = fork();

    if(pid == 0)
        puts("I'm Child Proc\n");
    else 
    {
        printf("Child Proc PID: %d\n", pid);
        sleep(30);
    }

    if(pid == 0) 
    {
        printf("Child Proc End\n");
    }
    else 
    {
        printf("Parent Proc End\n");
    }
    return 0;
}