#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF 1024

void error(char*message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sock;

    char message[BUF];
    int str_len;
    struct sockaddr_in serv_adr;

    if(argc != 3)
    {
        printf("Usage: %s <IP> <sock>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        error("sock() error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));

    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error("connect() error");
    else
        puts("Connected....");
    
    while(1)
    {
        fputs("Input message(Q, q): ",stdout);
        fgets(message, BUF, stdin);

        if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
        {
            printf("프로그램을 종료합니다.");
            break;
        }

        write(sock, message, strlen(message));
        str_len = read(sock, message, BUF - 1);
        message[str_len] = 0;
        printf("서버로부터 온 메세지: %s", message);
    }
    close(sock);

    return 0;
}