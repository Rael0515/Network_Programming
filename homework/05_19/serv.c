#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <signal.h>

#include <sys/wait.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void read_childproc(int sig)
{
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    if (pid > 0)
        printf("자식 프로세스 종료 (pid: %d)\n", pid);
}

int main(int argc, char* argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;

    int fds1[2], fds2[2];
    pid_t pid_a, pid_b;
    struct sigaction act;
    socklen_t adr_sz;

    int state;
    char name[100];
    FILE *fp_init;

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);
    if (state == -1)
        error_handling("sigaction() error");

    printf("저장할 파일 이름을 입력하시오: ");
    scanf("%s", name);

    fp_init = fopen(name, "wt");
    if (fp_init == NULL)
        error_handling("fopen() error");
    fclose(fp_init);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    while (1)
    {
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
        if (clnt_sock == -1)
            continue;
        printf("새 클라이언트가 연결되었습니다!\n");

        pipe(fds1), pipe(fds2);
        pid_a = fork();

        if (pid_a == 0)
        {
            FILE *fp;
            char msgbuf[BUFSIZE];
            int len;

            close(serv_sock);
            close(clnt_sock);
            close(fds1[1]);
            close(fds2[0]);

            fp = fopen(name, "at");
            if (fp == NULL)
                error_handling("file open error");

            while ((len = read(fds1[0], msgbuf, BUFSIZE - 1)) > 0)
            {
                msgbuf[len] = 0;

                fwrite(msgbuf, 1, len, fp);
                write(fds2[1], msgbuf, len);

                if (msgbuf[0] == 'q' || msgbuf[0] == 'Q')
                    break;
            }
            fclose(fp);
            close(fds1[0]);
            close(fds2[1]);
            exit(0);
        }

        pid_b = fork();

        if (pid_b == 0)
        {
            char msgbuf[BUFSIZE];
            int len;

            close(serv_sock);
            close(fds1[0]);
            close(fds2[1]);

            while ((len = read(clnt_sock, msgbuf, BUFSIZE - 1)) > 0)
            {
                msgbuf[len] = 0;
                printf("[자식B] 클라이언트 메세지: %s", msgbuf);

                write(fds1[1], msgbuf, len);

                len = read(fds2[0], msgbuf, BUFSIZE - 1);
                if (len <= 0)
                    break;
                msgbuf[len] = 0;

                if (msgbuf[0] == 'q' || msgbuf[0] == 'Q')
                {
                    write(clnt_sock, "End of Message", 14);
                    printf(">>클라이언트에게 종료메세지 전송<<\n");
                    break;
                }
                else
                    write(clnt_sock, msgbuf, len);
            }
            close(clnt_sock);
            close(fds1[1]);
            close(fds2[0]);
            puts("클라이언트 연결이 종료되었습니다.");
            exit(0);
        }
        close(clnt_sock);
        close(fds1[0]); close(fds1[1]);
        close(fds2[0]); close(fds2[1]);
    }
    close(serv_sock);
    return 0;
}

