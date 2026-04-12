#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[])
{
    char *addr="127.232.124.79";
    struct sockaddr_in addr_inet;

    if(!inet_aton(addr, &addr_inet.sin_addr))
        error_handling("주소 에러");
    else
        printf("원래의 주소: %s", addr);
        printf("네트워크 주소는: %#x입니다.\n", addr_inet.sin_addr.s_addr);
    return 0;
}