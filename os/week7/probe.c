#include <stdio.h>

#define __LINUX

#ifdef __LINUX  // 进入 Linux 系统
    #include <stdlib.h>
    #include <string.h>
    #include <dirent.h>

int main(int argc, char const *argv[])
{
    char line[256];
    int text_found = 0, data_found = 0, heap_found = 0, stack_found = 0, bss_found = 0;

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
        else if (!bss_found && strstr(line, "rw-p") != NULL)
        {
            printf("\tBSS\n");
            bss_found = 1;
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

#else // 进入 Windows 系统
    #include <windows.h>

int main()
{
    HANDLE hProcess = GetCurrentProcess();
    MEMORY_BASIC_INFORMATION mbi;
    DWORD_PTR addr = 0;

    DWORD_PTR segments[5] = {0};
    char* segment_names[5] = {"text", "data", "BSS", "heap", "stack"};
    int segment_index = 0;

    while (VirtualQuery((LPCVOID)addr, &mbi, sizeof(mbi)) != 0)
    {
        if (mbi.State == MEM_COMMIT && mbi.Type == MEM_PRIVATE)
            if (mbi.Protect & PAGE_EXECUTE_READ)
                segments[segment_index ++ ] = (DWORD_PTR)mbi.BaseAddress;
            else if (mbi.Protect & PAGE_READWRITE) 
                if (addr == (DWORD_PTR)mbi.BaseAddress)
                    segments[segment_index ++ ] = (DWORD_PTR)mbi.BaseAddress;
                else
                    segments[segment_index ++ ] = (DWORD_PTR)mbi.BaseAddress;
            else if (mbi.Protect & PAGE_READONLY)
                segments[segment_index ++ ] = (DWORD_PTR)mbi.BaseAddress;
        addr += mbi.RegionSize;
    }

    printf("In a Windows system, output the relative positions of each segment from low address to high address sequentially:\n");
    for (int i = 0; i < 5; i ++ )
        for (int j = 0; j < 5; j ++ )
        {
            if (segments[i] == 0)
                break;
            if (segments[j] == 0)
                continue;
            if (segments[i] == segments[j])
            {
                printf("\t%s\n", segment_names[j]);
                segments[j] = 0;
                break;
            }
        }

    return 0;
}

#endif
