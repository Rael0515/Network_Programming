#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<arpa/inet.h>
#include<sys/socket.h>

#define buf 1024

void error(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char*argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    char opinfo;
    int num[buf];
    int result, opnd_cnt, recv_cnt, recv_len, i;

    if(argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error("socket() error");
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error("bind() error");
    if(listen(serv_sock, 5) == -1)
        error("listen() error");
    clnt_adr_sz = sizeof(clnt_adr);

    for(i = 0; i < 5; i ++)
    {
        opnd_cnt = 0;
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        read(clnt_sock, &opnd_cnt, 1);

        recv_len = 0;
        int target_len = opnd_cnt * sizeof(int);
        while(target_len > recv_len)
        {
            recv_cnt = read(clnt_sock, (char*)num + recv_len, target_len - recv_len);
            recv_len += recv_cnt;
        }
        read(clnt_sock, &opinfo, 1);

        result = num[0];
        for(int j = 1; j < opnd_cnt; j++)
        {
            switch(opinfo)
            {
                case '+': result += num[j]; break;
                case '-': result -= num[j]; break;
                case '*': result *= num[j]; break;
            }
        }
        write(clnt_sock, &result, sizeof(result));
        close(clnt_sock);
    }
}