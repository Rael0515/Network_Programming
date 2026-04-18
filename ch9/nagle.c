#include <stdio.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

int main(void)
{
    int sock;
    int opt_val;
    socklen_t opt_len;

    sock = socket(PF_INET, SOCK_STREAM, 0);

    opt_len = sizeof(opt_val);
    getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void*)&opt_val, &opt_len);
    printf("Nagle state: %s \n", (opt_val == 0) ? "ON" : "OFF");
    
    opt_val = 1;
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void*)&opt_val, sizeof(opt_val));
    
    getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void*)&opt_val, &opt_len);

    printf("Nagle state: %s \n", (opt_val == 0) ? "ON" : "OFF");

    close(sock);
    return 0;
}