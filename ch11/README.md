## 第 11 章 进程间通信

本章代码，在[TCP-IP-NetworkNote](https://github.com/riba2534/TCP-IP-NetworkNote)中可以找到。

进程间通信，意味着两个不同的进程中可以交换数据

### 11.1 进程间通信的基本概念

#### 11.1.1 通过管道实现进程间通信

下图是基于管道（PIPE）的进程间通信的模型：

![](https://s2.ax1x.com/2019/01/22/kFlk0s.png)

可以看出，为了完成进程间通信，需要创建进程。管道并非属于进程的资源，而是和套接字一样，属于操作系统（也就不是 fork 函数的复制对象）。所以，两个进程通过操作系统提供的内存空间进行通信。下面是创建管道的函数。

```c
#include <unistd.h>
int pipe(int filedes[2]);
/*
成功时返回 0，失败时返回 -1
filedes[0]: 通过管道接收数据时使用的文件描述符，即管道出口
filedes[1]: 通过管道传输数据时使用的文件描述符，即管道入口
*/
```

父进程创建函数时将创建管道，同时获取对应于出入口的文件描述符，此时父进程可以读写同一管道。但父进程的目的是与子进程进行数据交换，因此需要将入口或出口中的 1 个文件描述符传递给子进程。下面的例子是关于该函数的使用方法：

- [pipe1.c](./pipe1.c)

编译运行：

```shell
gcc pipe1.c -o pipe1
./pipe1
```

结果：

```
Who are you?
```

可以从程序中看出，首先创建了一个管道，子进程通过 fds[1] 把数据写入管道，父进程从 fds[0] 再把数据读出来。可以从下图看出：

![](https://s2.ax1x.com/2019/01/22/kF8A7d.png)

#### 11.1.2 通过管道进行进程间双向通信

下图可以看出双向通信模型：

![](https://s2.ax1x.com/2019/01/22/kF84De.png)

下面是双向通信的示例：

- [pipe2.c](./pipe2.c)

编译运行：

```shell
gcc pipe2.c -o pipe2
./pipe2
```

结果：

```
Parent proc output: Who are you?
Child proc output: Thank you for your message
```

运行结果是正确的，但是如果注释掉第18行的代码，就会出现问题，导致一直等待下去。

> 向管道传递数据时，先读的进程会把数据取走。

因为数据进入管道后变成了无主数据。也就是通过 read 函数先读取数据的进程将得到数据，即使该进程将数据传到了管道。因此注释掉子进程的`sleep(2)`会产生问题。子进程读回自己向管道发送的数据。结果父进程调用 read 函数后将无限期等待数据进入管道。

当一个管道无法完成双向通信任务时，就需要创建两个管道，各自负责不同的数据流动，过程如下图所示：

![](https://s2.ax1x.com/2019/01/22/kFJW0e.png)

下面采用上述模型改进 `pipe2.c` 。

- [pipe3.c](./pipe3.c)

上面通过创建两个管道实现了功能，此时，不需要额外再使用 sleep 函数。运行结果和上面一样。

### 11.2 运用进程间通信

#### 11.2.1 保存消息的回声服务器

下面对第 10 章的 [echo_mpserv.c](https://github.com/riba2534/TCP-IP-NetworkNote/blob/master/ch10/echo_mpserv.c) 进行改进，添加一个功能：

> 将回声客户端传输的字符串按序保存到文件中

实现该任务将创建一个新进程，从向客户端提供服务的进程读取字符串信息，下面是代码：

- [echo_storeserv.c](./echo_storeserv.c)

编译运行：

```shell
gcc echo_storeserv.c -o serv
./serv 9190
```

此服务器端配合第 10 章的客户端 [echo_mpclient.c](https://github.com/riba2534/TCP-IP-NetworkNote/blob/master/ch10/echo_mpclient.c) 使用，运行结果如下图:

![](https://s2.ax1x.com/2019/01/22/kFUCct.png)

![](https://s2.ax1x.com/2019/01/22/kFUAHS.png)

从图上可以看出，服务器端已经生成了文件，把客户端的消息保存可下来，只保存了10次消息。

### 11.3 习题

> 以下答案仅代表本人个人观点，可能不是正确答案。

1. **什么是进程间通信？分别从概念和内存的角度进行说明。**

   答：
   
   进程间通信（Inter-Process Communication，IPC）是指在不同进程之间传递数据的机制。由于每个进程都有自己独立的内存空间，因此一个进程不能直接访问另一个进程的内存。这就需要一种机制来在进程之间传递数据。

   管道是一种半双工的通信方式，数据只能单向流动，且只能在具有亲缘关系的进程间使用。**管道是一段共享的内存**，一个进程向管道中写入数据，另一个进程从管道中读取数据。

2. **进程间通信需要特殊的 IPC 机制，这是由操作系统提供的。进程间通信时为何需要操作系统的帮助？**

   答：为了进行进程间通信，需要管道的帮助，但是管道不是进程的资源，它属于从操作系统，所以，两个进程通过操作系统提供的内存空间进行通信。

3. **「管道」是典型的 IPC 技法。关于管道，请回答以下问题：**

   1. **管道是进程间交换数据的路径。如何创建此路径？由谁创建？**

      答：使用 pipe 函数进行创建，由操作系统创建。父进程调用该函数时将创建管道。

   2. **为了完成进程间通信。2 个进程要同时连接管道。那 2 个进程如何连接到同一管道？**

      答：
      1. 创建管道：使用pipe()系统调用创建一个新的管道。pipe()函数会返回两个文件描述符，一个用于读取，一个用于写入。

      2. 创建子进程：使用fork()系统调用创建一个新的子进程。子进程会继承父进程的文件描述符，因此父子进程都可以访问同一个管道。

      3. 父子进程分别关闭不需要的文件描述符：父进程如果只负责写入数据，就关闭读取端的文件描述符；子进程如果只负责读取数据，就关闭写入端的文件描述符。

      4. 父子进程通过管道进行通信：父进程通过写入端的文件描述符向管道写入数据，子进程通过读取端的文件描述符从管道读取数据。

   3. 管道允许 2 个进程间的双向通信。双向通信中需要注意哪些内容？

      答：向管道传输数据时，先读的进程会把数据取走。简言之，就是数据进入管道后会变成无主数据，所以有时候为了防止错误，需要多个管道来进程通信。
