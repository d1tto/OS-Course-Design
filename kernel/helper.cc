#include "helper.h"



void wrmsr(uint32_t ecx,uint32_t high,uint32_t low)
{
     asm volatile(
        "wrmsr;"
        ::"d"(high),"a"(low),"c"(ecx)
    );
}

void rdmsr(uint32_t ecx,uint32_t* high,uint32_t *low)
{
    asm volatile(
        "rdmsr;"
        :"=d"(*high),"=a"(*low) //output
        :"c"(ecx) //input
    );
}

void puts(const char* str)
{
    asm volatile(
        "xor rax,rax;"
        "mov rbx,%[str];"
        "start_loop:"
        "mov al, byte ptr[rbx];"
        "cmp al,0;"
        "jz exit_loop;"
        "out dx,al;"
        "inc rbx;"
        "jmp start_loop;"
        "exit_loop: "
        "mov rax,0xa;"
        "out dx,al;"
        ::[str]"b"(str)
    );
}
void read_str(char* ptr,uint64_t size)
{
    asm volatile(
        "xor rax,rax;"
        "mov rbx,%[ptr];"
        "mov rcx,%[size];"
        "read_str_loop_start:"
        "cmp rcx,0;"
        "jz read_str_done;"
        "in al,dx;"
        "mov byte ptr[rbx],al;"
        "inc rbx;"
        "dec rcx;"
        "jmp read_str_loop_start;"
        "read_str_done: nop;"
        ::[ptr]"b"(ptr),[size]"c"(size)
    );
}

void write_str(char* ptr,uint64_t size)
{
    asm volatile(
        "xor rax,rax;"
        "mov rbx,%[ptr];"
        "mov rcx,%[size];"
        "write_str_loop_start:"
        "cmp rcx,0;"
        "jz write_str_done;"
        "mov al,byte ptr[rbx];"
        "out dx,al;"
        "inc rbx;"
        "dec rcx;"
        "jmp write_str_loop_start;"
        "write_str_done: nop;"
        ::[ptr]"b"(ptr),[size]"c"(size)
    );
}

void strcpy(char* dest,const char* src)
{
    while ((*dest++=*src++)!='\0');
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

char* strcat(char *dst, const char *src)
{
    char *temp = dst;
    while (*temp != '\0')
        temp++;
    while ((*temp++ = *src++) != '\0');

    return dst;
}