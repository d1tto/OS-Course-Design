#include "kernel.h"

extern uint64_t syscall_entry;

extern uint64_t kernel_stack;

Kernel::Kernel()
{
    //init_pagetable();
    setup_syscall();
}

void 
Kernel::switch_to_user()
{
    asm volatile(
        "mov [rip+kernel_stack],rsp;"
        "mov rcx,%[shell_start];"
        "mov r11, 0x2;"
        "xor rax, rax;"
        "xor rbx, rbx;"
        "xor rdx, rdx;"
        "xor rdi, rdi;"
        "xor rsi, rsi;"
        "xor rbp, rbp;"
        "xor r8, r8;"
        "xor r9, r9;"
        "xor r10, r10;"
        "xor r12, r12;"
        "xor r13, r13;"
        "xor r14, r14;"
        "xor r15, r15;"
        "mov rbp,%[user_stack];"
        "mov rsp,rbp;"
        "sub rsp,0x100;"
        ".byte 0x48;"
        "sysret;"
        ::[shell_start]"i"(SHELL_START),[user_stack]"i"(USER_STACK_BASE)
    );
}

void 
Kernel::setup_syscall()
{
    asm volatile(
        //IA32 STAR MSR  0xc0000081
        //CS SS 从这里加载固定值
        "xor rax, rax;"
        "mov rdx, 0x00200010;"
        "mov ecx, 0xc0000081;"
        "wrmsr;"


        // IA32 LSTAR MSR  0xc0000082
        // RIP = LSTAR
        // 执行syscall将跳转到syscall_entry
        "lea rdi,[rip+syscall_entry];"
        "mov rax,rdi;"
        "mov rdx,0;"
        "mov ecx,0xc0000082;"
        "wrmsr;"
    );
}
