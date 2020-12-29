#ifndef _KERNEL_HELPER_H
#define _KERNEL_HELPER_H
#include<stdint.h>

#define KERNEL_START 0
#define RAM_DISK_START 0x200000

#define MEM_SIZE (0x400000)
#define KERNEL_STACK_BASE (MEM_SIZE/2-0x1000)
#define KERNEL_SIZE (0x4000)

#define PAGE_TABLE_SIZE (0x5000)
#define PAGE_TABLE_START (KERNEL_STACK_BASE - KERNEL_SIZE - PAGE_TABLE_SIZE)
#define ONE_PAGE_SIZE (1024 * 1024 * 2)

#define SHELL_START 0x200000
#define OTHER_START 0x300000
#define USER_STACK_BASE (0x600000-0x1000)
#define DISK_START 0x600000
#define DYNMEM_START 0xa00000


extern "C"
{
void wrmsr(uint32_t ecx,uint32_t high,uint32_t low);
void rdmsr(uint32_t ecx,uint32_t* high,uint32_t *low);

void puts(const char* str);
void read_str(char* ptr,uint64_t size);
void write_str(char* ptr,uint64_t size);
void strcpy(char* dest,const char* src);
uint64_t strlen(const char* buf);
int32_t strcmp(const char* str1,const char* str2);
char* strcat(char *dst, const char *src);
char *digitToAlpha (int val, char *buf, unsigned radix);
}
#endif