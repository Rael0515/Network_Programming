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

int main(int argc, char*argv[])
{
    int serv_sock, clnt_sock;
    FILE *fp;

    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    char message[buf];
    int read_cnt;

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
        error("listen error");
    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

    fp = fopen("file_server2.c", "rb");
    while(1)
    {
        read_cnt = fread((void*)message, 1, buf, fp);
        if(read_cnt < buf)
        {
            write(clnt_sock, message, read_cnt);
            break;
        }
        write(clnt_sock, message, buf);
    }

    shutdown(clnt_sock, SHUT_WR);
    read(clnt_sock, message, buf);
    printf("Message from client: %s\n", message);

    fclose(fp);
    close(clnt_sock);
    close(serv_sock);
    return 0;

}