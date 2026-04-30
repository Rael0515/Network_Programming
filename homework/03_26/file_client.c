#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    FILE *fp;
    char request_name[BUF_SIZE];
    char save_name[BUF_SIZE];
    char buf[BUF_SIZE];
    int read_cnt;
    struct sockaddr_in serv_adr;

    if(argc!=3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    printf("Download file name: ");
    scanf("%s", request_name);
    printf("Save file name: ");
    scanf("%s", save_name);

    sock=socket(PF_INET, SOCK_STREAM, 0);   
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_adr.sin_port=htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        error_handling("connect() error!");

    write(sock, request_name, strlen(request_name) + 1);

    int is_received = 0;
    fp = fopen(save_name, "wb");
    
    while((read_cnt = read(sock, buf, BUF_SIZE)) != 0)
    { 
        fwrite((void*)buf, 1, read_cnt, fp);
        is_received = 1;
    }

    if(is_received)
        printf("Successfully send '%s' and saved '%s'.\n", request_name, save_name);
    else {
        printf("File not found. Connection closed.\n");
        remove(save_name); 
    }

    fclose(fp);
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
