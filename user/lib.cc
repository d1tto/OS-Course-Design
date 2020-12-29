#include "lib.h"

uint64_t read(char* buf,uint64_t size)
{
    asm volatile(
        "mov rdi,%[buf];"
        "mov rsi,%[size];"
        "mov rax,0;"
        "syscall;"
        ::[buf]"r"(buf),[size]"r"(size)
    );

}

uint64_t write(const char* buf,uint64_t size)
{
    asm volatile(
        "mov rdi,%[buf];"
        "mov rsi,%[size];"
        "mov rax,1;"
        "syscall;"
        ::[buf]"r"(buf),[size]"r"(size)
    );
}

void puts(const char* str)
{
    write(str,strlen(str));
    write("\n",1);
}

void gets(char* str){
    while(true){
        read(str,1);
        if(*str == '\n'){
            *str = '\x00';
            break;
        }
        str++;
    }
}

uint64_t strlen(const char* buf)
{
    int i = 0;
    for(i = 0 ; buf[i] ; i++);
    return i;
}

int32_t strcmp(const char* str1,const char* str2)
{
    if(strlen(str1) != strlen(str2))
        return 1;
    int size = strlen(str1);
    for(int i = 0 ; i < size ; i++){
        if(str1[i]!=str2[i])
            return 1;
    }
    return 0;
}

int32_t atoi(const char* src)
{
    int s = 0;
    bool isMinus = false;

    while(*src == ' ')  //跳过空白符
        src++; 
    if(*src == '+' || *src == '-')
    {
        if(*src == '-')
            isMinus = true;
        src++;
    }
    else if(*src < '0' || *src > '9')
    {
        s = 2147483647;
        return s;
    }
    while(*src != '\0' && *src >= '0' && *src <= '9')
    {
        s = s * 10 + *src - '0';
        src++;
    }
    return s * (isMinus ? -1 : 1);
}

char *strsep(char **stringp, const char *delim)
{
    char *s;
    const char *spanp;
    int c, sc;
    char *tok;
    if ((s = *stringp)== nullptr)
        return (nullptr);
    for (tok = s;;) {
        c = *s++;
        spanp = delim;
        do {
            if ((sc =*spanp++) == c) {
                if (c == 0)
                    s = nullptr;
                else
                    s[-1] = 0;
                *stringp = s;
                return (tok);
            }
        } while (sc != 0);
    }
    /* NOTREACHED */
}


char *digitToAlpha (int val, char *buf, unsigned radix) 
{ 
    char *p; /* pointer to traverse string */ 
    char *firstdig;/* pointer to first digit */ 
    char temp; /* temp char */ 
    unsigned digval; /* value of digit */ 
    p = buf; 
    if(val<0)
    { 
        /* negative, so output '-' and negate */ 
        *p++= '-'; 
        val = (unsigned long)(-(long)val); 
    } 
    firstdig = p;/* save pointer to first digit */ 
    do { 
        digval = (unsigned)(val%radix); 
        val /=radix; /* get next digit */ 
        /* convert to ascii and store */ 
        if (digval > 9) 
            *p++ = (char) (digval - 10 + 'a'); /* a letter */ 
        else 
            *p++ = (char) (digval + '0'); /* a digit */ 
    } while(val > 0); 
    /* We now have the digit of the number in the buffer, but in reverse 
    order. Thus we reverse them now. */ 
    *p-- = '\0'; /* terminate string; p points to last digit */ 
    do 
    { 
        temp = *p; 
        *p =*firstdig; 
        *firstdig= temp; /* swap *p and *firstdig */ 
        --p; 
        ++firstdig;     /* advance to next two digits */ 
    } while (firstdig < p); /* repeat until halfway */ 
    return buf; 
}

void strcpy(char* dest,const char* src)
{
    while ((*dest++=*src++)!='\0');
}