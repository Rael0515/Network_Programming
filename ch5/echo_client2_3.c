#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define buf 1024

void error(char* m)
{
    fputs(m, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char*argv[])
{
    int sock;
    char message[buf];
    int str_len, recv_len, recv_cnt;
    struct sockaddr_in serv_adr;

    if(argc != 3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        error("error");
    }
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error("erro");
    else
        puts("connected....");
    while(1)
    {
        fputs("인풋메세지: ", stdout);
        fgets(message, buf,stdin);

        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;
        str_len = write(sock, message, sizeof(message));

        recv_len = 0;
        while(recv_len < str_len)
        {
            recv_cnt = read(sock, &message[recv_len], buf -1);
            if(recv_cnt == -1)
                error("error");
            recv_len+=recv_cnt;
        }

        message[recv_len] = 0;
        printf("%s", message);
    }
    close(sock);
    return 0;
}