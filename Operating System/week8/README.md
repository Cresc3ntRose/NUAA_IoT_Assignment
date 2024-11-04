# 本周作业

Go 语言是一门非常优秀的语言，具有与 C、CPP 同等量级的性能，与此同时规避了不安全指针、内存泄露等问题. 然而，Go 语言最大的特色是从语言层面支持并发(Goroutine)，调研 Goroutine 的底层原理，形成研究报告.


### solution
具体内容请[点击这里](./week8.pdf).
在研究报告的基础之上，我参考了南京大学[蒋炎岩](https://jyywiki.cn/)2024年春季学期的[操作系统](https://jyywiki.cn/OS/2024/)课程，完成了实验[M2](https://jyywiki.cn/OS/2024/labs/M2.md)，实现了一个小型的协程库，具体实现见[`./libco`](./libco/co.c)文件夹.

* 注意：`./libco` 中的代码无法直接运行，需要将这段代码放至[南京大学2024操作系统实验](https://jyywiki.cn/OS/2024/labs/Labs.md)环境下，才可正确运行.