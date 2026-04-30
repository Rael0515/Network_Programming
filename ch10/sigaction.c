#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
    if(sig == SIGALRM)
        puts("Time out!");
    if(sig == SIGINT)
        puts("\nCTRL+C pressed");
    alarm(2);
}

int main(int argc, char*argv)
{
    int i;
    struct sigaction act;
    act.sa_handler = timeout;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGALRM, &act, 0);
    sigaction(SIGINT, &act, 0);

    alarm(2);

    for(int i = 0; i < 3; i++)
    {
        puts("wait...");
        sleep(100);
    }
    return 0;
}