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

int main(int argc, char*argv[])
{
    int sock;
    int str_len;
    char message[buf];

    struct sockaddr_in serv_adr, from_adr;
    socklen_t adr_sz;


    if(argc != 3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
        error("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
    
    /*
    struct sockaddr_in temp_adr;
	memset(&temp_adr, 0, sizeof(temp_adr));
	temp_adr.sin_family=AF_INET;
	temp_adr.sin_addr.s_addr=inet_addr("0.0.0.0");
	temp_adr.sin_port=htons(54192);

	bind(sock, (struct sockaddr *)&temp_adr, sizeof(temp_adr));
    */
    while(1)
    {
        fputs("Input message(quit to q or Q): ", stdout);
        fgets(message,sizeof(message), stdin);
        if(!strcmp(message, "q\n")||!strcmp(message, "Q\n"))
            break;
        
        adr_sz = sizeof(serv_adr);
        sendto(sock, message, strlen(message), 0, (struct sockaddr*)&serv_adr, adr_sz);
        
        printf("serv_adr\n");
		printf("IP: %s PORT: %d\n", inet_ntoa(serv_adr.sin_addr), ntohs(serv_adr.sin_port));
        
        adr_sz = sizeof(from_adr);

        str_len = recvfrom(sock, message, buf, 0, (struct sockaddr*)&from_adr, &adr_sz);
        printf("recvfrom\n");
        printf("IP: %s PORT: %d\n", inet_ntoa(from_adr.sin_addr), ntohs(from_adr.sin_port));
        message[str_len] = 0;
        printf("Message from server: %s", message);
    }
    close(sock);
    return 0;
}