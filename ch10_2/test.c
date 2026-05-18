#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    printf("한번만 출력됩니다.\n");
    fork();
    printf("두번 출력됩니다.\n");
    fork();
    printf("몇번 출력될까요?\n");
    fork();
    printf("그만해 미친놈아\n");
    return 0;
}