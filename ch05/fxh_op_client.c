#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char* argv[]){
    int sock;
    int str_len;
    struct sockaddr_in serv_adr;

    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) error_handling("socket() error");
    else printf("serv_sock create\n");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error!");
    else printf("connected\n");

    int num;
    char operator;

    fputs("input nums length: ", stdout);
    scanf("%d", &num);
    write(sock, &num, sizeof(num));
    for (int i = 0; i < num; i++){
        printf("input num[%d]: ", i);
        scanf("%d", &num);
        write(sock, &num, sizeof(num));
    }
    printf("input operator: ");
    scanf("%c", &operator);
    write(sock, &operator, sizeof(operator));

    close(sock);
    return 0;
}

// ./fxh_op_client 127.0.0.1 10000