#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define BUF_SIZE 1024

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    struct timeval timeout;
    fd_set reads, cpy_reads;

    socklen_t adr_sz;
    int fd_max, str_len, fd_num, i;
    char buf[BUF_SIZE];

    FILE *fp[BUF_SIZE] = {0};
    char file_name[20];

    if (argc != 2)
    {
        printf("Usage: %s <port>", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_adr, 0, sizeof(serv_adr));

    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    FD_SET(0, &reads);
    fd_max = serv_sock;

    while(1)
    {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        if((fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout)) == -1)
            break;

        if(fd_num == 0)
            continue;

        for(i =0; i < fd_max + 1; i++)
        {
            if(FD_ISSET(i, &cpy_reads))
            {
                if(i == serv_sock)
                {
                    adr_sz = sizeof(clnt_adr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                    FD_SET(clnt_sock, &reads);
                    if(fd_max < clnt_sock)
                        fd_max = clnt_sock;
                    
                    sprintf(file_name, "%d.txt", clnt_sock);
                    fp[clnt_sock] = fopen(file_name, "w");

                    printf("connected client: %d\n", clnt_sock);
                }
                else if(i == 0)
                {
                    str_len = read(0, buf, BUF_SIZE - 1);
                    buf[str_len] = 0;
                    printf("Standard Input: %s\n", buf);
                }
                else
                {
                    str_len = read(i, buf, BUF_SIZE);
                    if(str_len == 0)
                    {
                        FD_CLR(i, &reads);
                        if(fp[i] != NULL)
                        {
                            fclose(fp[i]);
                            fp[i] = NULL;
                        }
                        close(i);
                        printf("closed client: %d\n", i);
                    }
                    else
                    {
                        buf[str_len] = '\0';

                        if(fp[i] != NULL)
                        {
                            fputs(buf, fp[i]);
                            fflush(fp[i]);
                        }
                        if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
                        {
                            write(i, "End of Message\n", 15);

                            FD_CLR(i, &reads);
                            if(fp[i] != NULL)
                            {
                                fclose(fp[i]);
                                fp[i] = NULL;
                            }
                            close(i);
                            printf("Closed client: %d\n", i);
                        }
                        else
                            write(i, buf, str_len);
                    }
                }
            }
        }
    }
    close(serv_sock);
    return 0;
}
