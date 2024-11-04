# 本周作业

正式版作业内容请[点击这里](./week11.pdf).

---

<!-- ## 作业0
* 如果有两个生产者，两个消费者，请改写下面的程序，消除竞争条件.

```c
semaphore mutex = 1, empty = n, full = 0;

void producer()
{
    while (true)
    {
        produce_an_item;
        wait(empty);
        wait(mutex);
        buffer[in] = item;
        in = (in + 1) % n;
        signal(mutex);
        signal(full);
    }
}

void consumer()
{
    while (true)
    {
        wait(full);
        item = buffer[out];
        out = (out + 1) % n;
        signal(empty);
        consume_the_item;
    }
}

```

### [solution](./code/data_race_solution.c):

在消费者函数中，由于修改 `out` 时并未对其上锁，可能会存在多个消费者同时修改 `out` 的情况. 可以引入一个新的信号量 `omutex`，用于保护 `out` 的修改.

--- -->

## 作业1
* 桌上有一空盘，只允许存放一个水果. 爸爸专向盘中放橙子，妈妈专向盘中放苹果，女儿专等吃橙子，儿子专等吃苹果. 规定当盘空时一次只能放一个水果供吃者自用，请用 P、V 操作实现爸爸、妈妈、女儿、儿子四个并发进程的同步.

### [solution](./code/fruit.c):
这里我参考了[蒋炎岩课上的示例](https://jyywiki.cn/OS/2024/lect10.md)，使用信号量实现同步.

注意:
* `fruit.c` 中的头文件 `thread.h` 与 `thread-sync.h` 来自[蒋炎岩](https://jyywiki.cn/)老师所提供的[最小线程库](https://jyywiki.cn/OS/2024/lect5.md)，执行
```
wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/thread-lib/"
```
后即可得到.

---

## 作业2
* 假设有个南北向的桥，仅能容同方向的人顺序走过，相对方向的两个人则无法通过。现在桥南北端都有过桥人. 现把每个过桥人当成一个进程，用 P、V 操作实现管理.

### [solution](./code/bridge.c):
与上一题类似，同样可以通过信号量来实现管理.
