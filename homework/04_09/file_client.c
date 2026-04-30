#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sd;
    FILE *fp;
    char file_name[BUF_SIZE];
    char buf[BUF_SIZE];
    int read_cnt;
    struct sockaddr_in serv_adr;

    if(argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    printf("Input upload file name: ");
    scanf("%s", file_name);

    fp=fopen(file_name, "rb");
    if(fp == NULL) {
        printf("File Not found\n");
        return 0;
    }

    sd = socket(PF_INET, SOCK_STREAM, 0);   
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        error_handling("connect() error!");

    while(1)
    {
        read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
        if(read_cnt < BUF_SIZE)
        {
            write(sd, buf, read_cnt);
            break;
        }
        write(sd, buf, BUF_SIZE);
    }

    shutdown(sd, SHUT_WR);
    
    read(sd, buf, BUF_SIZE);
    printf("server message: %s\n", buf);

    fclose(fp);
    close(sd);
    return 0;
}