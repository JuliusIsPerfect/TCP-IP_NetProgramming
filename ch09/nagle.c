#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sock, state;
    int opt_val;
    socklen_t optlen;
    struct sockaddr_in serv_adr;
    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    optlen = sizeof(opt_val);
    // TCP_NODELAY -> netinet/tcp.h
    state = getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void*)&opt_val, &optlen);
    printf("TCP_NODELAY = %d\n", opt_val);

    opt_val = 1;
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void*) &opt_val, optlen);
    
    state = getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void*)&opt_val, &optlen);
    
    printf("TCP_NODELAY = %d\n", opt_val);
    close(sock);
    return 0;
}