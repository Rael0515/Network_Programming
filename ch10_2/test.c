#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid = fork();
    if (pid == 0)
    {
        puts("안녕 난 자식 프로세스야");
        puts("난 이제 집에 가고싶어졌어\n안녕");
        exit(0);
    }
    else
    {
        puts("안녕 난 부모 프로... zzz");
        wait(NULL);
        puts("어라 내 프로세스 어디갔지?");
    }

    return 0;

}