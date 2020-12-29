#ifndef MY_KERNEL_H
#define MY_KERNEL_H

#include "helper.h"
#include "disk.h"

class Kernel
{
    public:
        Kernel();
        void switch_to_user();
    private:
        void setup_syscall();
};

#endif