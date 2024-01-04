#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void read_childproc(int sig)
{
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    printf("removed proc id: %d \n", pid);
}

int main(){
    int fds1[2], fds2[2];
    char* str[3];
    pid_t pid;

    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    int state = sigaction(SIGCHLD, &act, 0);

    str[0] = "Hi! I'm 0.";
    str[1] = "Hi! I'm 1.";
    str[2] = "Bye!";

    pipe(fds1);
    pipe(fds2);

    pid = fork();

    if (pid == 0){
        char *str_child = malloc(BUF_SIZE);
        int len_child;
        write(fds1[1], str[0], strlen(str[0]));
        len_child = read(fds2[0], str_child, BUF_SIZE);
        printf("len_child = %d, received: %s\n", len_child, str_child);
    }
    else{
        char *str_father = malloc(BUF_SIZE);
        int len_father;
        len_father = read(fds1[0], str_father, BUF_SIZE);
        write(fds2[1], str[1], strlen(str[1]));
        printf("len_father = %d, received: %s\n", len_father, str_father);
    }




    return 0;
}