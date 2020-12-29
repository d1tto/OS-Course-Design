#ifndef USER_LIB_H
#define USER_LIB_H

#include<stdint.h>


uint64_t read(char* buf,uint64_t size);
uint64_t write(const char* buf,uint64_t size);
void puts(const char* str);
void gets(char* str);
uint64_t strlen(const char* buf);
int32_t strcmp(const char* str1,const char* str2);
int32_t atoi(const char* str1);
char *strsep(char **stringp, const char *delim);
char *digitToAlpha (int val, char *buf, unsigned radix);
void strcpy(char* dest,const char* src);
#endif