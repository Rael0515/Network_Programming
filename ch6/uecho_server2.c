#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define buf 1024

void error(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);

    exit(1);
}

int main(int argc, char* argv[])
{
    int serv_sock;
    char message[buf];
    int str_len;

    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock == -1)
        error("socket() error");
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr("0.0.0.0");
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error("bind() error");
    
    while(1)
    {
        clnt_adr_sz = sizeof(clnt_adr);
        str_len = recvfrom(serv_sock, message, buf, 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        message[str_len] = 0;

        
        printf("IP: %s, PORT: %d\n", inet_ntoa(clnt_adr.sin_addr), ntohs(clnt_adr.sin_port));
        printf("recvfrom: %s", message);
        
        sendto(serv_sock, message, str_len, 0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);
        printf("sendto\n");
        printf("IP: %s, PORT: %d\n", inet_ntoa(clnt_adr.sin_addr), ntohs(clnt_adr.sin_port));
    }
    close(serv_sock);
    return 0;
}