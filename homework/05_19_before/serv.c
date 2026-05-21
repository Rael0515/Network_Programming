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
    printf("removed porc id: %d\n", pid);
}

int main(int argc, char* argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;

    int fds1[2], fds2[2];
    pid_t pid;
    struct sigaction act;
    socklen_t adr_sz;

    int str_len, state;
    char name[100], buf[BUFSIZE];
    
    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    //act
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);


    printf("작성할 파일 이름을 적으시오: ");
    scanf("%s", name);

    //sock, addr 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));

    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    if(listen(serv_sock , 5) == -1)
        error_handling("listen() error");

    //파이프라인구성
    
    pid = fork(); // fork
    if(pid == 0)
    {
        FILE* fp = fopen(name, "wt");
        char msgbuf[BUFSIZE];
        int len;
        while(1)
        {
            len = read(fds1[0], msgbuf, BUFSIZE);
            if(len <= 0) 
                break;
            fwrite((void*)msgbuf, 1, len, fp);

            write(fds2[1], msgbuf, len); 

            if(!strcmp(msgbuf, "q\n")||!strcmp(msgbuf, "Q\n"))
                break;//q입력 == 탈출
        }
        fclose(fp);
        return 0;
    }
    while(1)
    {
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
        if(clnt_sock == -1)
            continue;
        else
            puts("새 클라이언트가 연결되었습니다...");
        
        pipe(fds1), pipe(fds2);

        pid = fork();
        if(pid == 0) 
        {
            FILE* fp = fopen(name, "at"); 
            char msgbuf[BUFSIZE];
            int len;
            
            
            while(1) 
            {
                len = read(fds1[0], msgbuf, BUFSIZE);
                if(len <= 0) break;
                
                fwrite((void*)msgbuf, 1, len, fp);
                write(fds2[1], msgbuf, len);
                
                if(!strcmp(msgbuf, "q\n")||!strcmp(msgbuf, "Q\n"))
                    break;
            }
            fclose(fp);
            return 0;
        }

        pid = fork();
        if(pid == 0) 
        {
            close(serv_sock);
            
        
            while((str_len = read(clnt_sock, buf, BUFSIZE)) != 0)
            {
                write(fds1[1], buf, str_len);
                str_len = read(fds2[0], buf, BUFSIZE);
                
                if(!strcmp(buf, "q\n")||!strcmp(buf, "Q\n"))
                {
                    write(clnt_sock, "End of Message", 14);
                    break;
                }
                else
                {
                    write(clnt_sock, buf, str_len);
                }
            }
            close(clnt_sock);
            puts("클라이언트 연결 해제...");
            return 0;
        }
        else
        {
            close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}
}