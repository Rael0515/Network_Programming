#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

//모든 함수는 에러가 나올시 -1을 반환
int main(int argc, char *argv[])
{
    int serv_sock;//소켓의 번호(파일디스크립터)
    int clnt_sock;//클라이언트와 연결 시 사용할 소켓의 번호표

    struct sockaddr_in serv_addr;//IP, Port를 담는 구조체
    struct sockaddr_in clnt_addr;//연결된 클라이언트의 주소 정보를 담을 구조체
    socklen_t clnt_addr_size;//주소 구조체의 크기를 저장하는 변수

    char message[] = "Good At Play"; // 클라이언트에게 보내는 데이터

    if(argv != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    
    serv_sock = socket(PF_INET, SOCK_STREAM, 0); //소켓생성, PF_INET: IPv4체계, SOCK_STREAM: TCP, UDP는 SOCK_DGRAM
    memset(&serv_addr, 0, sizeof(serv_addr));
    //구조체를 0으로 깨끗히 비움

    serv_addr.sin_family = AF_INET; //주소체계 IPv4
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //내 PC의 IP를 넣어줌
    serv_addr.sin_port = htons(atoi(argv[1])); //문자열 포트를 숫자로 변경해서 입력

    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr) == -1)) //바인드오류
        error_handling("bind() error");
    if(listen(serv_sock, 5) == -1) //리슨오류
        error_handling("listen() error");

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*) &serv_addr, &clnt_addr_size);
    if(clnt_sock == -1)
        error_handling("accept() error");

    write(clnt_sock, message, sizeof(message));
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}