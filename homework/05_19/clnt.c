#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <signal.h>

#include <sys/wait.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}


int main(int argc, char *argv[])
{
    int sock;
    char message[BUFSIZE];
    int str_len;
    struct sockaddr_in serv_adr;

    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error!");
    else
        puts("서버에 연결되었습니다!");

    while (1)
    {
        fputs("(q or Q to quit) 메세지 입력 >> ", stdout);
        fgets(message, BUFSIZE, stdin);

        write(sock, message, strlen(message));

        str_len = read(sock, message, BUFSIZE - 1);
        if (str_len <= 0)
            break;
        message[str_len] = 0;

        if (!strcmp(message, "End of Message"))
        {
            printf("서버로부터 받은 메세지: %s\n", message);
            puts("클라이언트 종료.");
            break;
        }
        else
        {
            printf("서버로부터 받은 메세지: %s\n", message);
        }
    }

    close(sock);
    return 0;
}