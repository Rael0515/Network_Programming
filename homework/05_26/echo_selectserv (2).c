#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	struct timeval timeout;
	fd_set reads, cpy_reads;

	socklen_t adr_sz;
	int fd_max, str_len, fd_num, i;
	char buf[BUF_SIZE];

	/* 소켓 번호별로 저장할 파일 포인터를 보관하는 배열 */
	FILE *fp_list[FD_SETSIZE] = {0};
	char file_name[20];

	if(argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);
	FD_SET(0, &reads);            /* 표준입력(0)도 감시 대상에 추가 */
	fd_max = serv_sock;

	while(1)
	{
		cpy_reads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if((fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout)) == -1)
			break;

		if(fd_num == 0)
			continue;

		for(i = 0; i < fd_max + 1; i++)
		{
			if(FD_ISSET(i, &cpy_reads))
			{
				if(i == serv_sock)
				{
					/* 연결 요청 수락 */
					adr_sz = sizeof(clnt_adr);
					clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
					FD_SET(clnt_sock, &reads);
					if(fd_max < clnt_sock)
						fd_max = clnt_sock;

					/* [요구사항 2] accept 이후 "소켓번호.txt" 파일 생성 */
					sprintf(file_name, "%d.txt", clnt_sock);
					fp_list[clnt_sock] = fopen(file_name, "w");

					printf("connected client: %d\n", clnt_sock);
				}
				else if(i == 0)
				{
					/* [요구사항 1] 표준입력 데이터를 읽어서 화면에 출력 */
					str_len = read(0, buf, BUF_SIZE - 1);
					buf[str_len] = '\0';
					printf("Standard Input: %s", buf);
				}
				else
				{
					/* 클라이언트가 보낸 데이터 처리 */
					str_len = read(i, buf, BUF_SIZE);
					if(str_len == 0)        /* 연결 종료 */
					{
						FD_CLR(i, &reads);
						if(fp_list[i] != NULL)
						{
							fclose(fp_list[i]);
							fp_list[i] = NULL;
						}
						close(i);
						printf("closed client: %d\n", i);
					}
					else
					{
						buf[str_len] = '\0';

						/* 받은 문자열을 해당 소켓 번호의 파일에 저장 */
						if(fp_list[i] != NULL)
						{
							fputs(buf, fp_list[i]);
							fflush(fp_list[i]);
						}

						/* [요구사항 3] q 또는 Q 메시지 처리 */
						if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
						{
							write(i, "End of Message\n", 15);

							FD_CLR(i, &reads);
							if(fp_list[i] != NULL)
							{
								fclose(fp_list[i]);
								fp_list[i] = NULL;
							}
							close(i);
							printf("closed client: %d\n", i);
						}
						else
						{
							/* 일반 메시지는 그대로 에코 */
							write(i, buf, str_len);
						}
					}
				}
			}
		}
	}
	close(serv_sock);
	return 0;
}
