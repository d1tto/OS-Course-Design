#ifndef _KERNEL_SYSCALL_H
#define _KERNEL_SYSCALL_H
#include <stdint.h>
#include "helper.h"
#include "filesystem.h"

#define SYS_ERROR -1
#define SYS_OK 0

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
    SYS_execve,
    SYS_rm,
    SYS_cd_parent,
    SYS_loadfile,
    syscall_END
};

using syscall_func = uint64_t(*)(uint64_t,uint64_t,uint64_t);


extern "C" uint64_t syscall_handler(uint64_t arg0, uint64_t arg1, uint64_t arg2);

#define MAKE_SYSCALL_FUNC(name) \
    uint64_t name(uint64_t arg0, uint64_t arg1, uint64_t arg2)

MAKE_SYSCALL_FUNC(sys_read);
MAKE_SYSCALL_FUNC(sys_write);
MAKE_SYSCALL_FUNC(sys_listdir);
MAKE_SYSCALL_FUNC(sys_readfile);
MAKE_SYSCALL_FUNC(sys_writefile);
MAKE_SYSCALL_FUNC(sys_create_dir);
MAKE_SYSCALL_FUNC(sys_create_file);
MAKE_SYSCALL_FUNC(sys_change_dir);
MAKE_SYSCALL_FUNC(sys_current_dir);
MAKE_SYSCALL_FUNC(sys_execve);
MAKE_SYSCALL_FUNC(sys_loadfile);
MAKE_SYSCALL_FUNC(sys_rm);
#endif