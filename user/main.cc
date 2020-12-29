#include "lib.h"

enum syscall_num{
    SYS_read,
    SYS_write,
    SYS_listdir,
    SYS_readfile,
    SYS_writefile,
    SYS_create_dir,
    SYS_create_file,
    SYS_change_dir,
    SYS_current_dir,
    SYS_rm,
    SYS_execve,
    SYS_loadfile,
    syscall_END
};

using command_func = void(*)(char*);

struct command
{
    const char* name;
    command_func func_ptr;
};

#define CMD_NUMB 9

#define MAKE_CMD_FUNC(name) \
    void name(char* arg_ptr)

MAKE_CMD_FUNC(cmd_ls);
MAKE_CMD_FUNC(cmd_cat);
MAKE_CMD_FUNC(cmd_exec);
MAKE_CMD_FUNC(cmd_pwd);
MAKE_CMD_FUNC(cmd_mkdir);
MAKE_CMD_FUNC(cmd_cd);
MAKE_CMD_FUNC(cmd_writefile);
MAKE_CMD_FUNC(cmd_touch);
MAKE_CMD_FUNC(cmd_rm);
command cmd_table[] = {
    {"ls",cmd_ls},
    {"cat",cmd_cat},
    {"exec",cmd_exec},
    {"pwd",cmd_pwd},
    {"mkdir",cmd_mkdir},
    {"touch",cmd_touch},
    {"cd",cmd_cd},
    {"writefile",cmd_writefile},
    {"rm",cmd_rm},
};
#define _OK 1
#define _NOT_EXSIT 0
#define _IS_DIR 2
char* skip_space(char* ptr){
    for(;*ptr == ' ';ptr++);
    return ptr;
}
MAKE_CMD_FUNC(cmd_rm)
{
    int64_t res;
    asm volatile(
        "mov rdi,%[name_ptr];"
        "mov rax,%[touch_numb];"
        "syscall;"
        "mov %[res],rax;"
        :[res]"=r"(res)
        :[name_ptr]"r"(arg_ptr),[touch_numb]"i"(SYS_rm)
    );
    if(res == false){
        puts("No such file or directory");
    }
}
MAKE_CMD_FUNC(cmd_touch){
    int64_t res;
    asm volatile(
        "mov rdi,%[name_ptr];"
        "mov rax,%[touch_numb];"
        "syscall;"
        "mov %[res],rax;"
        :[res]"=r"(res)
        :[name_ptr]"r"(arg_ptr),[touch_numb]"i"(SYS_create_file)
    );
    if(res == false){
        puts("file exist");
    }
}

MAKE_CMD_FUNC(cmd_ls){
    asm volatile(
        "mov rax,%[SYS_listdir];"
        "syscall;"
        ::[SYS_listdir]"i"(SYS_listdir)
    );
}

MAKE_CMD_FUNC(cmd_mkdir)
{
    int64_t res;
    asm volatile(
        "mov rdi,%[name_ptr];"
        "mov rax,%[mdkir_numb];"
        "syscall;"
        "mov %[res],rax;"
        :[res]"=r"(res)
        :[name_ptr]"r"(arg_ptr),[mdkir_numb]"i"(SYS_create_dir)
    );
    if(res == false){
        puts("DIR exists");
    }
}

MAKE_CMD_FUNC(cmd_cd)
{
    int64_t res;
    asm volatile(
        "mov rdi,%[name_ptr];"
        "mov rax,%[cd_numb];"
        "syscall;"
        "mov %[res],rax;"
        :[res]"=r"(res)
        :[name_ptr]"r"(arg_ptr),[cd_numb]"i"(SYS_change_dir)
    );
    if(res == false){
        puts("No such file or directory");
    }
}

MAKE_CMD_FUNC(cmd_cat){
    int64_t res;
    char buf[0x100] = {0};
    asm volatile(
        "mov rdi,%[name_ptr];"
        "mov rsi,%[output_ptr];"
        "mov rax,%[cat_numb];"
        "syscall;"
        "mov %[res],rax;"
        :[res]"=r"(res)
        :[name_ptr]"r"(arg_ptr),[cat_numb]"i"(SYS_readfile),[output_ptr]"r"(buf)
    );
    if(res == _NOT_EXSIT){
        puts("No such file or directory");
    }
    else if(res == _IS_DIR){
        puts("file is DIR");
    }
    else
        puts(buf);
}

MAKE_CMD_FUNC(cmd_exec){
    puts("exec test");
}

char* get_cur_dir(){
    char *ptr = nullptr;
    asm volatile(
        "mov rax,%[SYS_current_dir];"
        "syscall;"
        "mov %[ptr],rax;"
        :[ptr]"=r"(ptr)
        :[SYS_current_dir]"i"(SYS_current_dir)
    );
}

MAKE_CMD_FUNC(cmd_pwd)
{
    char *ptr = get_cur_dir();
    puts(ptr);
}

//writefile name content
MAKE_CMD_FUNC(cmd_writefile)
{
    int64_t res;

    char name[0x10] = {0};
    
    char* cur = arg_ptr;
    char* end = cur;
    while(true){//end指向字符串的cmd的末尾
            if(*end == '\x00')
                break;
            if(*end == ' ')
                break;
            end++;
        }
    *end = '\x00';
    strcpy(name,cur);
    end = skip_space(end+1);

    asm volatile(
        "mov rdi,%[name_ptr];"
        "mov rsi,%[intput];"
        "mov rax,%[write_numb];"
        "syscall;"
        "mov %[res],rax;"
        :[res]"=r"(res)
        :[name_ptr]"r"(name),[write_numb]"i"(SYS_writefile),[intput]"r"(end)
    );
    if(res == _NOT_EXSIT){
        puts("No such file or directory");
    }
    else if(res == _IS_DIR){
        puts("file is DIR");
    }
}


int main(){
    char cmd[0x100] = {0};
    while(true){
        char wel[] = "babykernel@ditto:";
        write(wel,strlen(wel));
        char* cur_ptr = get_cur_dir();
        write(cur_ptr,strlen(cur_ptr));
        write("$ ",2);

        //Zero Memory
        for(int i = 0 ; i < 0x100-1 ; i++)
            cmd[i] = 0;
        gets(cmd);
        char* cur = skip_space(cmd);//跳过开头的空格

        char* end = cur;
        while(true){//end指向字符串的cmd的末尾
            if(*end == '\x00')
                break;
            if(*end == ' ')
                break;
            end++;
        }
        *end = '\x00';

        end = skip_space(end+1);
        int flag = false;
        for(int i = 0 ; i < CMD_NUMB ; i++){
            if(!strcmp(cur,cmd_table[i].name)){
                flag = true;
                cmd_table[i].func_ptr(end);
                break;
            }
        }
        if(!flag){
            write("command \"",10);
            write(cur,strlen(cur));
            write("\" not found\n",12);
        }
    }
}

