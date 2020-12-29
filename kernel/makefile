cc := g++
CFLAGS := -m64 -mcmodel=large -masm=intel -nostdlib -ffreestanding -std=c++11 -fno-exceptions  -nostdlib -fno-rtti -Wall -Wextra
LDFLAGS := -T linkscript.ld
kernel.bin: kernel_main.cc entry.o syscall_entry.o helper.o kernel.o syscall.o disk.o malloc.o filesystem.o
	$(cc) $(CFLAGS) entry.o kernel_main.cc kernel.o helper.o syscall_entry.o syscall.o disk.o filesystem.o malloc.o -o kernel.bin $(LDFLAGS)

entry.o : entry.S 
	$(cc) $(CFLAGS) -c entry.S -o entry.o

syscall_entry.o: syscall_entry.S
	$(cc) $(CFLAGS) -c syscall_entry.S -o syscall_entry.o

helper.o : helper.cc
	$(cc) $(CFLAGS) -c helper.cc -o helper.o

kernel.o : kernel.cc
	$(cc) $(CFLAGS) -c kernel.cc -o kernel.o

syscall.o : syscall.cc
	$(cc) $(CFLAGS) -c syscall.cc -o syscall.o

disk.o : disk.cc
	$(cc) $(CFLAGS) -c disk.cc -o disk.o

malloc.o : malloc.cc
	$(cc) $(CFLAGS) -c malloc.cc -o malloc.o

filesystem.o : filesystem.cc
	$(cc) $(CFLAGS) -c filesystem.cc -o filesystem.o

clean:
	rm *.o