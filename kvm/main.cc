#include "kvm.h"

int main(int argc,char**argv)
{
    if(argc < 3){
        cout<<"[Usage] "<<argv[0]<<" filename"<<endl;
        return 0;
    }
    Hypervisor vm(argv[1],argv[2]);
    vm.Run();
}