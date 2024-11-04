#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*func_ptr_t)();

void text_segment_function()
{
    int a = 0;
}

int main()
{
    char stack_buffer[100] = "Hello";
    printf("Stack:\t");
    stack_buffer[0] == 'H' ? printf("r") : printf("-");
    strcpy(stack_buffer, "Hello, Stack!") ? printf("w") : printf("-");
    // void (*stack_func)() = (void (*)())stack_buffer;
    // stack_func(); // 程序崩溃，说明不可执行
    // printf("x\n");
    printf("-\n");
    
    char* heap_buffer = (char*)calloc(15, 1);
    printf("Heap:\t");
    heap_buffer[0] == 0 ? printf("r") : printf("-");
    strcpy(heap_buffer, "Hello, Hheap!") ? printf("w") : printf("-");
    // void (*heap_func)() = (void (*)())heap_buffer;
    // heap_func(); // 程序崩溃，说明不可执行
    // printf("x\n");
    printf("-\n");

    static char data_buffer[100] = "Hello, Data!";
    printf("Data:\t");
    data_buffer[0] == 'H' ? printf("r") : printf("-");
    strcpy(data_buffer, "Data changed!") ? printf("w") : printf("-");
    // void (*data_func)() = (void (*)())data_buffer;
    // data_func(); // 程序崩溃，说明不可执行
    // printf("x\n");
    printf("-\n");

    func_ptr_t text_func = text_segment_function;
    text_func();
    printf("text: \tr");
    // char* text_buffer = (char*)text_segment_function;
    // text_buffer[0] = 'X'; // 段错误,说明不可写
    // printf("w");
    printf("-");
    printf("x\n");

    free(heap_buffer);

    return 0;
}
