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

enum SEG {
    TEXT,
    DATA,
    BSS,
    HEAP,
    STACK
};

typedef struct segment_info_t
{
    char* name;
    DWORD start_address;
    int SEG;
} segment_info_t;

int main() {
    segment_info_t segments[] = {
        {"text", 0, TEXT},
        {"data", 0, DATA},
        {"bss", 0, BSS},
        {"heap", 0, HEAP},
        {"stack", 0, STACK}
    };

    int text_found = 0, data_found = 0, heap_found = 0, bss_found = 0;
    HANDLE hProcess = GetCurrentProcess();
    MEMORY_BASIC_INFORMATION mbi;
    DWORD_PTR addr = 0;

    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    segments[STACK].start_address = (DWORD_PTR)sysinfo.lpMaximumApplicationAddress;

    while (VirtualQuery((LPCVOID)addr, &mbi, sizeof(mbi)) != 0)
    {
        if (mbi.State == MEM_COMMIT && mbi.Type == MEM_PRIVATE)
            if (mbi.Protect & PAGE_EXECUTE_READ)
                for (int i = 0; i < 5; i ++ )
                    if (segments[i].SEG == TEXT && !text_found)
                    {
                        segments[i].start_address = (DWORD_PTR)mbi.BaseAddress;
                        text_found = 1;
                        break;
                    }
            else if (mbi.Protect & PAGE_READWRITE)
                if (mbi.AllocationBase == mbi.BaseAddress)
                    for (int i = 0; i < 5; i ++ )
                        if (segments[i].SEG == DATA && !data_found)
                        {
                            segments[i].start_address = (DWORD_PTR)mbi.BaseAddress;
                            data_found = 1;
                            break;
                        }
                else
                    for (int i = 0; i < 5; i ++ )
                        if (segments[i].SEG == HEAP && !heap_found)
                        {
                            segments[i].start_address = (DWORD_PTR)mbi.BaseAddress;
                            heap_found = 1;
                            break;
                        }
            else if (mbi.Protect & PAGE_READONLY)
                for (int i = 0; i < 5; i ++ )
                    if (segments[i].SEG == BSS && !bss_found)
                    {
                        segments[i].start_address = (DWORD_PTR)mbi.BaseAddress;
                        bss_found = 1;
                        break;
                    }
        addr += mbi.RegionSize;
    }

    printf("In a Windows system, output the relative positions of each segment from low address to high address sequentially:\n");
    for (int i = 0; i < 5; i ++ )
        for (int j = 0; j < 4 - i; j ++ )
            if (segments[j].start_address > segments[j + 1].start_address)
            {
                segment_info_t temp = segments[j];
                segments[j] = segments[j + 1];
                segments[j + 1] = temp;
            }

    for (int i = 0; i < 5; i++)
        printf("\t%s\n", segments[i].name);

    return 0;
}

#endif
