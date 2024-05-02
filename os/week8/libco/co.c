#include "co.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>

#define STACK_SIZE (32 * 1024) // uintptr_t ---->  8 in x86_64

// 切换堆栈后执行函数调用，且能传递一个参数
static inline
	void stack_switch_call(void *sp,
						   void *entry,
						   uintptr_t arg)
{
	asm volatile(
	#if __x86_64__
		"movq %%rsp,-0x10(%0); leaq -0x20(%0), %%rsp; movq %2, %%rdi ; call *%1; movq -0x10(%0) ,%%rsp;"
		:
		: "b"((uintptr_t)sp), "d"(entry), "a"(arg)
		: "memory"
	#else
		"movl %%esp, -0x8(%0); leal -0xC(%0), %%esp; movl %2, -0xC(%0); call *%1;movl -0x8(%0), %%esp"
		:
		: "b"((uintptr_t)sp), "d"(entry), "a"(arg)
		: "memory"
	#endif
	);
}

enum co_status // 协程状态
{
    CO_NEW = 1, // 新创建，还未执行过
    CO_RUNNING, // 已经执行过
    CO_WAITING, // 在 co_wait 上等待
    CO_DEAD,    // 已经结束，但还未释放资源
};

struct co // 协程结构
{
	void (*func)(void *); // co_co 指定的入口地址和参数
	void *arg;
	char name[50];

    enum co_status status;  // 协程的状态
    struct co *    waiter;  // 是否有其他协程在等待当前协程
    struct co *    next;    // 协程池中的下一个协程
    jmp_buf        context; // 寄存器现场 (setjmp.h)
    uint8_t        stack[STACK_SIZE]; // 协程的堆栈    uint8_t == unsigned char
};

struct co *current; // 当前协程

int co_pool_size = 0; // 协程池中的协程个数

void co_pool_insert(struct co *co) // 将协程添加进协程池
{
	struct co *c = current;
	while (c)
	{
		if (c->next == current)
			break;

		c = c->next;
	}
	c->next = co;
	co->next = current;

	co_pool_size ++ ;
}

void co_pool_delete(struct co *co) // 将协程从协程池中删除
{
	struct co *c = current;
	while (c)
	{
		if (c->next == co)
			break;
		c = c->next;
	}
	c->next = c->next->next;
	free(co);

	co_pool_size -- ;
}

struct co *co_start(const char *name,
					void (*func)(void *),
					void *arg)
{
	struct co *co = (struct co *)malloc(sizeof(struct co));
	co->arg = arg;
	co->func = func;
	co->status = CO_NEW;
    strcpy(co->name, name);

	if (current == NULL) // 初始化 main 协程
	{
		current = (struct co *)malloc(sizeof(struct co));
		current->status = CO_RUNNING;
		current->waiter = NULL;
		strcpy(current->name, "main");
		current->next = current;
	}

	co_pool_insert(co);

	return co;
}

void co_wait(struct co *co)
{
	current->status = CO_WAITING;
	co->waiter = current;
	while (co->status != CO_DEAD)
	{
		co_yield ();
	}
	current->status = CO_RUNNING;

	co_pool_delete(co);
}
void co_yield ()
{
	if (current == NULL) // 初始化 main 协程
	{
		current = (struct co *)malloc(sizeof(struct co));
		current->status = CO_RUNNING;
        strcpy(current->name, "main");
		current->next = current;
	}

	int val = setjmp(current->context);
	if (val == 0)
	{
		struct co *c = current;
		do // 选择一个可以执行的协程
			c = c->next;
		while (c->status == CO_DEAD || c->status == CO_WAITING);

		current = c;

		if (current->status == CO_NEW)
		{
			current->status = CO_RUNNING;
			stack_switch_call(&current->stack[STACK_SIZE],
							current->func,
							(uintptr_t)current->arg);
			current->status = CO_DEAD;

			if (current->waiter)
				current = current->waiter;
		}
		else
			longjmp(current->context, 1);
	}
	else
		return;
}