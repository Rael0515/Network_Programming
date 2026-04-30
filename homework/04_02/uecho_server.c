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
    int serv_sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t clnt_adr_sz;
    struct sockaddr_in serv_adr, clnt_adr;

    if(argc!=2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    
    serv_sock=socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock==-1)
        error_handling("UDP socket creation error");
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(argv[1]));
    
    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        error_handling("bind() error");

    printf("Server started. Waiting for client...\n");

    while(1) 
    {
        clnt_adr_sz=sizeof(clnt_adr);
        str_len=recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        message[str_len] = 0;
        printf("Client: %s", message);
	if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
		printf("\nClient sent Q or q.\nYou have to send bye.\nIf you send bye, the server is close.\n\n");
        printf("Input reply: ");
        fgets(message, BUF_SIZE, stdin);
        
        sendto(serv_sock, message, strlen(message), 0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);

        if(!strcmp(message, "bye\n") || !strcmp(message, "bye")) {
            printf("You sent 'bye'.\n\nClosing server...\n");
            break;
        }
    }   
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
