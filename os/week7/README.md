# 本周作业

---

## 作业1
* 一般操作系统中，进程的每个段内部地址均连续，但段与段的相对次序可能不同.
* 用 C/CPP 语言写一个探测程序，探测 Windows、Linux 操作系统中进程的各段的相对位置(输出次序即可).

### [solution](./probe.c):
在 Linux 系统下，以进程 `System` 为例，其 pid 为 1，查看`/proc/1/maps` 文件，我们可以看到[其内容](./maps.txt).

不难发现，`maps` 文件中的地址呈现是由小到大的，所以在 Linux 系统下，我们按照地址从小到大的次序依次输出各段.

在[文件](./images/maps.png)中，我们可以很轻易地发现`[heap]`与`[stack]`字样，不难推断出这就是堆与栈的所在地址. 同时通过查阅资料，可以得知：
* 代码段通常以 `r-xp` 的权限标记出现.
* 数据段通常以 `r--p` 的权限标记出现.
* BSS 段通常以 `rw-p` 的权限标记出现.

于是我们不难得到如下思路：
* 打开 `/proc/1/maps` 文件并查看其内容，然后进行查询.
* 当查询到 `r-xp` 时，就输出 `text`，随后不再处理 `r-xp`.
* 当查询到 `r--p` 时，就输出 `data`，随后不再处理 `r--p`.
* 当查询到 `rw-p` 时，就输出 `BSS`，随后不再处理 `rw-p`.
* 当查询到 `[heap]` 时，就输出 `heap`，随后不再处理 `[heap]`.
* 当查询到 `[stack]` 时，就输出 `stack`，随后不再处理 `[steak]`.

而在 Windows 系统下，我们可以直接调用系统提供的API—— `Windows.h` 库来获得我们想要的信息：
* `GetCurrentProcess()` 函数可以获得当前进程的句柄.
* `VirtualQuery()` 函数会查询指定地址的内存区域信息，并将结果存储在 `MEMORY_BASIC_INFORMATION` 结构体 `mbi` 中.
* `GetSystemInfo()` 函数可以获取系统信息，其中 `lpMaximumApplicationAddress` 表示应用程序可以使用的最大地址，即栈段的起始地址.

于是我们可以沿用与 Linux 系统下相同的思路得到这个程序在 Windows 系统下的实现.
* 值得注意的是，我们并不能通过 Windows 系统 API 直接得到堆的地址，通常堆与 BSS 段均具有读写全权限，但区别在于，BSS 段的分配基址就是页面基址，而堆则不然，可以通过这个差别来得出这两个段的起始地址.

---

## 作业2
* 实现一程序，分别在 Windows、Linux 操作系统下验证：
  * 栈、堆、数据区是否可读可写不可执行.
  * 代码段是否可读不可写可执行.

### [solution](./checker.c):
可以使用C语言来进行验证.
##### 栈(Stack)
* 使用 `char stack_buffer[100] = "Hello";` 在栈上分配了一个 100 字节的缓冲区.
* 尝试修改第一个字符为 `'r'`(`stack_buffer[0] == 'H' ? printf("r") : printf("-");`)成功，说明栈区域可读写.
* 尝试用 `strcpy(stack_buffer， "Hello, Stack!");` 修改整个缓冲区内容，也成功，进一步证明栈区域可读写.
* 尝试将栈缓冲区转换为函数指针并调用(`stack_func();`)会导致程序崩溃，说明栈上的数据不可执行.
##### 堆(Heap)
* 使用 `char* heap_buffer = (char*)calloc(15， 1);` 在堆上分配了一个 15 字节的缓冲区，并初始化为 0.
* 尝试读取第一个字符(`heap_buffer[0] == 0 ? printf("r") : printf("-");`)成功，说明堆区域可读.
* 尝试用 `strcpy(heap_buffer， "Hello, Hheap!");` 修改整个缓冲区内容，也成功，进一步证明堆区域可读写.
* 尝试将堆缓冲区转换为函数指针并调用(`heap_func();`)会导致程序崩溃，说明堆上的数据不可执行.
##### 数据段(Data)
* 使用 `static char data_buffer[100] = "Hello, Data!";` 在数据段分配了一个 100 字节的缓冲区，并初始化.
* 尝试读取第一个字符(`data_buffer[0] == 'H' ? printf("r") : printf("-");`)成功，说明数据段可读.
* 尝试用 `strcpy(data_buffer， "Data changed!");` 修改整个缓冲区内容，也成功，进一步证明数据段可读写.
* 尝试将数据缓冲区转换为函数指针并调用(`data_func();`)会导致程序崩溃，说明数据段上的数据不可执行.
##### 代码段(Text)
* 定义了一个 `text_segment_function()` 函数，它位于代码段(text segment)中.
* 尝试将 `text_segment_function()` 的地址赋值给一个函数指针(`text_func = text_segment_function;`)并调用(`text_func();`)成功，说明代码段的数据可执行.
* 尝试修改代码段的内容(`text_buffer[0] = 'X';`)会导致段错误，说明代码段是只读的.