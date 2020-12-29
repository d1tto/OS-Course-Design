#include "kvm.h"
#include <unistd.h>

#define SHELL_START 0x200000
#define OTHER_START 0x300000
#define DISK_START 0x600000

Hypervisor::Hypervisor(std::string kfile,std::string user_file,size_t entry_point)
    :kernel_file_(kfile),user_file_(user_file),entry_point_(entry_point)
{
    init_vm();

    //读取代码到memptr_指向的内存中
    readfile_in_memory(kernel_file_,(char*)memptr_);

    //读取用户态程序(shell)到物理内存0x200000处
    readfile_in_memory(user_file_,(char*)memptr_+SHELL_START);
}

void 
Hypervisor::readfile_in_memory(std::string file_name,char* ptr)
{
    std::fstream fs(file_name,std::ios::in|std::ios::binary);
    fs.seekg(0,std::ios::end);
    file_size_ = fs.tellg();
    fs.seekg(0, std::ios::beg);
    fs.read(ptr,file_size_);
    fs.close();
}

// 虚拟地址：0 - 0x200000  -> 物理地址：0 - 0x200000
// 0x200000 -> 1  000000000 000000000000
void 
Hypervisor::setup_page()
{
    kvm_sregs sregs;
    ioctl(vcpu_fd_,KVM_GET_SREGS,&sregs);
    uint64_t pml4_paddr = PAGE_TABLE_START; //建立的页表在物理内存中的起始地址
    uint64_t *pml4 = (uint64_t*)((char*)memptr_ + pml4_paddr);

    uint64_t pdp_paddr = pml4_paddr+0x1000;
    uint64_t *pdp = (uint64_t*)((char*)memptr_ + pdp_paddr);

    uint64_t pd_paddr = pdp_paddr + 0x1000;
    uint64_t *pd = (uint64_t*)((char*)memptr_+pd_paddr);

    //pml4 or pdp or pd起始地址都必须是4k对齐的
    //所以直接赋值即可
    pml4[0] = PDE64_PRESENT | PDE64_RW | PDE64_USER | pdp_paddr;
    pdp[0] = PDE64_PRESENT | PDE64_RW | PDE64_USER | pd_paddr;
    pd[0] = PDE64_PRESENT | PDE64_RW | PDE64_PS; /* kernel only, no PED64_USER */
    pd[1] = PDE64_PRESENT | PDE64_RW | PDE64_USER | PDE64_PS | ONE_PAGE_SIZE;
    pd[2] = PDE64_PRESENT | PDE64_RW | PDE64_USER | PDE64_PS | ONE_PAGE_SIZE*2;
    pd[3] = PDE64_PRESENT | PDE64_RW | PDE64_USER | PDE64_PS | ONE_PAGE_SIZE*3;
    pd[4] = PDE64_PRESENT | PDE64_RW | PDE64_USER | PDE64_PS | ONE_PAGE_SIZE*4;
    pd[5] = PDE64_PRESENT | PDE64_RW | PDE64_USER | PDE64_PS | ONE_PAGE_SIZE*5;

    sregs.cr3 = pml4_paddr; //CR3保存的是pml4表的起始物理地址
    sregs.cr4 = CR4_PAE;
    sregs.cr4 |= CR4_OSFXSR | CR4_OSXMMEXCPT; /* enable SSE instruction */
    sregs.cr0 = CR0_PE | CR0_MP | CR0_ET | CR0_NE | CR0_WP | CR0_AM | CR0_PG;
    sregs.efer = EFER_LME | EFER_LMA;
    sregs.efer |= EFER_SCE; /* enable syscall instruction */
    ioctl(vcpu_fd_,KVM_SET_SREGS,&sregs);

}


//64位淡化了段机制，基地址和段限制都无了
void 
Hypervisor::setup_seg_regs()
{
    kvm_sregs sregs;
    ioctl(vcpu_fd_, KVM_GET_SREGS, &sregs);

    struct kvm_segment seg = {0};
    seg.base = 0;
    seg.limit = 0xffffffff;
    seg.selector = 1 << 3;
    seg.present = 1;
    seg.type = 0xb; /* Code segment */
    seg.dpl = 0; /* Kernel: level 0 */
    seg.db = 0;
    seg.s = 1;
    seg.l = 1; /* long mode */
    seg.g = 1;

    sregs.cs = seg;

    seg.type = 0x3; /* Data segment */
    seg.selector = 2 << 3;
    sregs.ds = sregs.es = sregs.fs = sregs.gs = sregs.ss = seg;
    ioctl(vcpu_fd_, KVM_SET_SREGS, &sregs); 
}

void
Hypervisor::Run()
{
    while(true)
    {
        ioctl(vcpu_fd_, KVM_RUN, NULL);
        switch (run_->exit_reason)
        {
        case KVM_EXIT_HLT:
            cout<<"KVM_EXIT_HLT"<<endl;
            return;
            break;
        case KVM_EXIT_IO:
            {
                if(run_->io.direction == KVM_EXIT_IO_OUT)
                    putchar(*(((char *)run_) + run_->io.data_offset));
                else if(run_->io.direction == KVM_EXIT_IO_IN){
                    *(((char *)run_) + run_->io.data_offset) = getchar();
                }
            }
            break;
        case KVM_EXIT_FAIL_ENTRY:
            cout<<"KVM_EXIT_FAIL_ENTRY"<<endl;
            return;
        case KVM_EXIT_INTERNAL_ERROR:
            cout<<"KVM_EXIT_INTERNAL_ERROR"<<endl;
            return;
        case KVM_EXIT_SHUTDOWN:
            cout<<"KVM_EXIT_SHUTDOWN"<<endl;
            return;
        default:
            break;
        }
    }
}

void
Hypervisor::set_reg()
{
    kvm_regs regs;
    if(ioctl(vcpu_fd_, KVM_GET_REGS, &regs) < 0)
        error("ioctl(KVM_GET_REGS)");
    regs.rip = entry_point_;
    regs.rsp = regs.rbp = KERNEL_STACK_BASE;
    regs.rflags = 0x2; // in x86 the 0x2 bit should always be set
    if(ioctl(vcpu_fd_, KVM_SET_REGS, &regs) < 0) // set registers
        error("ioctl(KVM_SET_REGS)");
    //ShowReg(regs);
}

void
Hypervisor::init_vm()
{
    //step1 : open kvm
    kvm_fd_ = open("/dev/kvm",O_RDWR|O_CLOEXEC);
    if(kvm_fd_ < 0 )
        error("open(/dev/kvm)");

    //step2 : create a VM
    vm_fd_ = ioctl(kvm_fd_,KVM_CREATE_VM,0);
    if(vm_fd_ < 0)
        error("ioctl(KVM_CREATE_VM)");

    size_t mem_size = 1024 * 1024 * 12; //10M 物理空间
    //映射一块内存，当作VM的物理内存
    memptr_ = mmap(0,mem_size,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    if(memptr_ == nullptr)
        error("mmap error");

    kvm_userspace_memory_region region = {
        .slot = 0,
        .flags = 0,
        .guest_phys_addr = 0,
        .memory_size = mem_size,
        .userspace_addr = (size_t)memptr_
    };
    ioctl(vm_fd_, KVM_SET_USER_MEMORY_REGION, &region);

    //step4 : Create virtual cpu
    vcpu_fd_ = ioctl(vm_fd_, KVM_CREATE_VCPU, 0);

    //step 5 : set up memory for vCPU
    size_t vcpu_size = ioctl(kvm_fd_,KVM_GET_VCPU_MMAP_SIZE, NULL);
    run_ = (kvm_run*)mmap(0,
                        vcpu_size,
                        PROT_READ | PROT_WRITE, 
                        MAP_SHARED,
                        vcpu_fd_,
                        0);
    set_reg();
    
    //进入 long mode
    setup_page();
    setup_seg_regs();
}