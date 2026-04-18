#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF 1024

void error(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc,char* argv[])
{
    int sock;
    char message[BUF];
    int str_len, recv_len, recv_cnt;
    struct sockaddr_in serv_adr;

    if(argc != 3)
    {
        printf("Usage: %s, <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        error("sock() error");
    }

    memset(&serv_adr, sizeof(serv_adr), 0);
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
    {
        error("connect() error");
    }
    else
        puts("connected.........");
    
    while(1)
    {
        fputs("Input: ",stdout);
        fgets(message, BUF, stdin);

        if(!strcmp(message, "q\n") || !strcmp(message,"Q\n"))
            break;
        str_len = write(sock, message, strlen(message));

        recv_len = 0;

        while(recv_len<str_len)
        {
            recv_cnt = read(sock, &message[recv_len],BUF -1);
            if(recv_cnt == -1)
                error("read() error!");
            recv_len += recv_cnt;
        }

        message[recv_len] = 0;
        printf("서버로부터 온 메세지: %s", message);
    }

    close(sock);
    return 0;
}