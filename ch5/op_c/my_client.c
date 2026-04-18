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

int main(int argc, char* argv[])
{
	int sock;
	char opmsg;
	int num[buf];
	int opnd_cnt, result, i;
	struct sockaddr_in serv_adr;

	if(argc != 3)
	{
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error("socket() error");
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error("connected error");
	else
		puts("connected...");

	fputs("계산 수: ", stdout);
	scanf("%d", &opnd_cnt);

	for(i = 0; i < opnd_cnt; i++)
	{
		printf("계산숫자 %d번째: ", i+1);
		scanf("%d", &num[i]);
	}
	fgetc(stdin);
	fputs("계산기호:", stdout);
	scanf("%c", &opmsg);
	char count_byte = (char)opnd_cnt;
    write(sock, &count_byte, 1);
	write(sock, num, opnd_cnt *sizeof(i));
	write(sock, &opmsg, 1);

	read(sock, &result, sizeof(result));

	printf("연산결과: %d\n", result);
	close(sock);
	return 0;

}