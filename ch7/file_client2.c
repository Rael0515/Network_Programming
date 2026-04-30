#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define buf 30

void error(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);

    exit(1);
}

int main(int argc, char *argv[])
{
    int sock;
    FILE *fp;

    char message[buf];
    int read_cnt;
    struct sockaddr_in serv_adr;
    if(argc!= 3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    fp = fopen("receive.dat", "wb");
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error("connect error");

    while((read_cnt = read(sock, message, buf))!= 0)
        fwrite((void*)message, 1, read_cnt, fp);

    puts("Received file data");
    write(sock, "Thank you", 10);
    fclose(fp);
    close(sock);
    return 0;
}