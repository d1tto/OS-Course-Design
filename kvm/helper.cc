#include "helper.h"

void error(const char* message){
    perror(message);
    exit(-1);
}

void ShowReg(kvm_regs &regs)
{
#ifdef DEBUG
    printf("RAX = %p\n",regs.rax);
    printf("RBX = %p\n",regs.rbx);
    printf("RCX = %p\n",regs.rcx);
    printf("RDX = %p\n",regs.rdx);
#endif
}