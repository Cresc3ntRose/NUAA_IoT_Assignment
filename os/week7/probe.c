#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int main(int argc, char const *argv[])
{
    char line[256];
    int text_found = 0, data_found = 0, heap_found = 0, stack_found = 0;

    DIR* dir = opendir("/proc/1");
    if (!dir)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    FILE* fp = fopen("/proc/1/maps", "r");
    if (!fp)
    {
        perror("fopen");
        goto release;
    }

    printf("In a Linux system, output the relative positions of each segment from low address to high address sequentially:\n");

    while (fgets(line, sizeof(line), fp))
        if (!text_found && strstr(line, "r-xp") != NULL)
        {
            printf("\ttext\n");
            text_found = 1;
        }
        else if (!data_found && strstr(line, "r--p") != NULL)
        {
            printf("\tdata\n");
            data_found = 1;
        }
        else if (!heap_found && strstr(line, "[heap]") != NULL)
        {
            printf("\theap\n");
            heap_found = 1;
        }
        else if (!stack_found && strstr(line, "[stack]") != NULL)
        {
            printf("\tstack\n");
            stack_found = 1;
        }

release:
    if (fp)
        fclose(fp);
    closedir(dir);

    return 0;
}