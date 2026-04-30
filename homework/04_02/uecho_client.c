#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t adr_sz;
    FILE *fp;
    
    struct sockaddr_in serv_adr, from_adr;
    if(argc!=3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    
    sock=socket(PF_INET, SOCK_DGRAM, 0);   
    if(sock==-1)
        error_handling("socket() error");
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_adr.sin_port=htons(atoi(argv[2]));

    fp = fopen("message.txt", "wt");
    if(fp == NULL)
        error_handling("File open error");

    while(1)
    {
        fputs("Insert message(q or Q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);     
        
        sendto(sock, message, strlen(message), 0, 
                    (struct sockaddr*)&serv_adr, sizeof(serv_adr));
        
        fprintf(fp, "Client: %s", message);

        adr_sz=sizeof(from_adr);
        str_len=recvfrom(sock, message, BUF_SIZE, 0, 
                    (struct sockaddr*)&from_adr, &adr_sz);
        
        message[str_len]=0;
        printf("Server: %s", message);
        fprintf(fp, "Server: %s\n", message);

        if(!strcmp(message, "bye\n") || !strcmp(message, "bye"))
	{
            printf("\nTerminating...\n");
            break;
        }
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
