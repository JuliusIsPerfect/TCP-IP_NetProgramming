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
    int serv_sock, clnt_sock;
    int str_len, recv_len, recv_cnt;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");
    else printf("serv_sock create\n");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    else printf("bind() success\n");

    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");
    else printf("listening\n");

    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
    if (clnt_sock == -1)
        error_handling("accept() error");
    else printf("clnt_sock created\n");

    char cnt_c, operator;
    int cnt;
    
    read(clnt_sock, &cnt, sizeof(cnt));
    printf("nums_cnt = %d\n", cnt);
    // cnt = cnt_c - '0';
    int nums[cnt];

    for (int i = 0; i < cnt; i++){
        read(clnt_sock, &nums[i], BUF_SIZE);
        printf("nums[%d] = %d\n", i, nums[i]);
        // nums[i] = cnt_c - '0';
    }

    read(clnt_sock, &operator, BUF_SIZE);
    printf("operator = %c\n", operator);

    int ans = 0;
    if (operator == '+'){
        for (int i = 0; i < cnt; i++) ans += nums[i];   
    }
    else if (operator == '-'){
        ans = nums[0];
        for (int i = 1; i < cnt; i++) ans -= nums[i];
    }
    else{
        ans = 1;
        for (int i = 0; i < cnt; i++) ans *= nums[i];
    }

    printf("%d\n", ans);
    write(clnt_sock, &ans, sizeof(ans));


    close(clnt_sock);
    close(serv_sock);
    return 0;
}