#include "helper.h"
#include "kernel.h"
#include "filesystem.h"

extern "C" 
{

void kernel_main()
{
    puts("[*] Init Kernel...");
    Kernel kernel;
    file_system = Filesystem();
    puts("[+] Init done");
    puts("[*] Switch to User");
    kernel.switch_to_user();

} 

}


