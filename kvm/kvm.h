#ifndef _KVM_H
#define _KVM_H

#include <fcntl.h>
#include <linux/kvm.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string>
#include <iostream>
#include <fstream>
#include "helper.h"

using std::cout;
using std::endl;

#define MEM_SIZE (0x400000)
#define KERNEL_STACK_BASE (MEM_SIZE/2-0x1000)
#define KERNEL_SIZE (0x4000)

#define PAGE_TABLE_SIZE (0x5000)
#define PAGE_TABLE_START (KERNEL_STACK_BASE - KERNEL_SIZE - PAGE_TABLE_SIZE)
#define ONE_PAGE_SIZE (1024 * 1024 * 2)

class Hypervisor
{
public:
    Hypervisor(std::string file,std::string user_file,size_t entry_point = 0);
    void Run();
private:
    void init_vm();
    void set_reg();
    void readfile_in_memory(std::string file_name,char* ptr);
    void setup_page();
    void setup_seg_regs();

    std::string kernel_file_;
    std::string user_file_;
    size_t file_size_;
    size_t entry_point_ = 0;
    int kvm_fd_; // open /dev/kvm 
    int vm_fd_;  // ioctl KVM_CREATE_VM
    int vcpu_fd_; // ioctl KVM_CREATE_VCPU
    void* memptr_; // mmap分配一块空间，用作VM的物理内存
    kvm_run* run_; // 每个VCPU对应一个kvm_run结构体

};


#endif