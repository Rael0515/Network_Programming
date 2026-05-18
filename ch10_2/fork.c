#include <stdio.h>
#include <unistd.h>

int gval = 10;

int main(int argc, char *argv[])
{
    pid_t pid;
    int lval = 20;
    gval++, lval += 5; //11, 25

    pid = fork();
    if(pid == 0) {gval+=2, lval+=2;}//13, 27
    else {gval-=2, lval -=2;}//9, 23

    if(pid == 0){printf("Child Porc: [%d, %d]", gval,lval);}
    else {printf("Parent Proc: [%d, %d]", gval,lval);}

    printf("\n");
    return 0;
}