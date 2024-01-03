#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
// ubuntu不加这个头文件，报错：不允许使用不完整的类类型 "struct sigaction"
// mac不需要加上这个头文件也可以创建sigaction，加上报错。
#include <bits/sigaction.h>

void read_childproc(int sig)
{
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status))
    {
        printf("Removed proc id: %d \n", id);             //子进程的 pid
        printf("Child send: %d \n", WEXITSTATUS(status)); //子进程的返回值
    }
}

int main(int argc, char *argv[])
{
    pid_t pid;
    // 注册 SIGCHLD 信号对应的处理器。若子进程终止，则调用 read_childproc。 
    // 处理函数中调用了waitpid函数，所以子进程将正常终止，不会成为僵尸进程。
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    pid = fork();
    if (pid == 0) //子进程执行阶段
    {
        puts("Hi I'm child process");
        sleep(10);
        return 12;
    }
    else //父进程执行阶段
    {
        printf("Child proc id: %d\n", pid);
        pid = fork();
        if (pid == 0)
        {
            puts("Hi! I'm child process");
            sleep(10);
            exit(24);
        }
        else
        {
            int i;
            printf("Child proc id: %d \n", pid);
            for (i = 0; i < 5; i++)
            {
                puts("wait");
                sleep(5);
            }
        }
    }
    return 0;
}
