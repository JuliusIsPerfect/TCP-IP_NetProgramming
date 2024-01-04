#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h> // 没有这个头文件就不能声明pid_t
#define BUF_SIZE 30

int main(int argc, char *argv[])
{
    int fds[2];
    /*
    char *str 创建的是一个指向字符串常量的指针，而下面 sizeof(str) 返回的是指针的大小，而不是字符串的长度（应该使用strlen）。
    这个值是 4 或 8（取决于你的系统是 32 位还是 64 位）。这种情况下，write 实际上只写入了字符串的一部分，最后就会输出乱码。
    char str[] 创建字符串，str 就是一个数组，而 sizeof(str) 就会返回整个数组的大小，也就是字符串的长度（包括结束符 '\0'）。
    */
    char str[] = "Who are you?";
    char buf[BUF_SIZE];
    pid_t pid;
    // 调用 pipe 函数创建管道，fds 数组中保存用于 I/O 的文件描述符
    pipe(fds);
    pid = fork(); // 子进程将同时拥有创建管道获取的2个文件描述符，复制的并非管道，而是文件描述符
    if (pid == 0)
    {
        write(fds[1], str, sizeof(str));
    }
    else
    {
        read(fds[0], buf, BUF_SIZE);
        puts(buf);
    }
    return 0;
}
