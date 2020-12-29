
.globl syscall_entry,kernel_stack
.extern puts
.extern syscall_handler
.intel_syntax noprefix

kernel_stack: .quad 0
user_stack: .quad 0

_str:
    .asciz "enter syscall"

// syscall将原RIP保存到RCX中，RFLAGS保存到R11中
// syscall不保存原RSP，由OS进行保存
syscall_entry:
    mov qword ptr [rip+user_stack],rsp //保存原RSP
    mov rsp,[rip+kernel_stack]         //切换到内核栈

    push rdi
    push rsi
    push rdx
    push rcx
    push r8
    push r9
    push r10
    push r11

    mov rcx, r10
    call syscall_handler

    pop r11
    pop r10
    pop r9
    pop r8
    pop rcx
    pop rdx
    pop rsi
    pop rdi

    mov rsp, [rip + user_stack]
    .byte 0x48
    sysret