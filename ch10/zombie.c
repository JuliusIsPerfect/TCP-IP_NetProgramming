#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    pid_t pid = fork();

    if (pid == 0)
    {
        puts("Hi, I am a child Process");
    }
    else
    {
        printf("Child Process ID: %d \n", pid);
        // 如果父进程终止，处于僵尸状态的子进程将同时销毁。因此，延缓父进程的执行以验证僵尸进程。
        sleep(30);
    }

    if (pid == 0)
        puts("End child proess");
    else
        puts("End parent process");
    return 0;
}
