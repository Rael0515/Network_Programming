#include <stdio.h>

#include <arpa/inet.h>

int main(int argc, char *argv)
{
    unsigned short host_port = 0x1234;
    unsigned short net_port;
    unsigned long host_addr = 0x12345678;
    unsigned long net_addr;

    net_port = htons(host_port);
    net_addr = htonl(host_addr);

    printf("호스트포트: %#x\n", host_port);
    printf("네트워크포트: %#x\n", net_port);
    printf("호스트주소: %#lx\n", host_addr);
    printf("네트워크주소: %#lx\n", net_addr);
}