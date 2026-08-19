#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void timetravel(int sig)
{

}

int main(int argc, char *argv[])
{
    struct sigaction act;

    act.sa_handler = timetravel;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, 0);
}