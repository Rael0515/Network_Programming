#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>

void error_handling(char* message)
{
    printf(message, stderr);
    printf('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[])
{
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;

    char message[] = "Hello";
}