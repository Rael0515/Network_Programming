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

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    char message[buf];
    int str_len, i;

    struct sockaddr_in serv_adr;
    struct sockaddr_in clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc != 2)
    {
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
    {
        error("socket() error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error("bind erropr");
    if(listen(serv_sock, 5) == -1)
        error("listen() error");
    
    clnt_adr_sz = sizeof(clnt_adr);

    for(i = 0; i <5; i++)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        if(clnt_sock == -1)
            error("accept() error");
        else
            printf("connected....");
        
        while((str_len = read(clnt_sock, message, buf)) == 0)
            write(clnt_sock, message, str_len);
        
        close(clnt_sock);
    }

    close(serv_sock);
    return 0;
}