#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    char *addr1="127.212.124.78";
	char *addr2="127.212.124.256";

    unsigned long conv_addr = inet_addr(addr1);
    printf("addr1 ");
    if(conv_addr == INADDR_NONE)
        printf("주소변환 오류!\n");
    else
        printf("IP 주소: %#lx\n", conv_addr);

    conv_addr = inet_addr(addr2);
    printf("addr2 ");
    if(conv_addr == INADDR_NONE)
       printf("주소변환 오류!\n");
    else
        printf("IP 주소: %#lx\n", conv_addr);
    
    return 0;
}