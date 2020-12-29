#include "syscall.h"

syscall_func syscall_table[]={
    sys_read,
    sys_write,
    sys_listdir,
    sys_readfile,
    sys_writefile,
    sys_create_dir,
    sys_create_file,
    sys_change_dir,
    sys_current_dir,
    sys_rm,
    sys_execve,
    sys_loadfile
};

uint64_t 
syscall_handler(uint64_t arg0, uint64_t arg1, uint64_t arg2) 
{
    uint32_t num;
    //获取系统调用号
    asm volatile(
        "mov %[num],eax;"
        :[num]"=r"(num)
    );
    //puts("enter syscall_handle");
    if(num >= syscall_END ){
        puts("no syscall function");
        return SYS_ERROR;
    }

    syscall_func func_ptr = syscall_table[num];
    return func_ptr(arg0,arg1,arg2);
}

// arg0 : buf_ptr
// arg1 : size
MAKE_SYSCALL_FUNC(sys_read){
    read_str((char*)arg0,arg1);
    return arg1;
}
MAKE_SYSCALL_FUNC(sys_write){
    write_str((char*)arg0,arg1);
    return arg1;;
}

MAKE_SYSCALL_FUNC(sys_listdir){
    file_system.list_dir();
    return SYS_ERROR;
}

MAKE_SYSCALL_FUNC(sys_create_dir)
{
    return file_system.create_dir((const char*)arg0);
}
MAKE_SYSCALL_FUNC(sys_rm)
{
    return file_system.rm_file((const char*)arg0);
}
// arg0 : file name
// arg1 : output
MAKE_SYSCALL_FUNC(sys_readfile){
    return file_system.read_file((const char*)arg0,(char*)arg1);
}

MAKE_SYSCALL_FUNC(sys_writefile)
{
    return file_system.write_file((const char*)arg0,(const char*)arg1);
}

MAKE_SYSCALL_FUNC(sys_change_dir)
{
    return file_system.change_dir((const char*)arg0);
}

MAKE_SYSCALL_FUNC(sys_current_dir)
{
    return (uint64_t)file_system.current_dir_name();
}

MAKE_SYSCALL_FUNC(sys_create_file)
{
    return file_system.create_file((const char*)arg0,0x50);
}

MAKE_SYSCALL_FUNC(sys_execve){
return SYS_ERROR;
}

MAKE_SYSCALL_FUNC(sys_loadfile){
    return SYS_ERROR;
}