cc := g++
CFLAGS := -m64 -mcmodel=large -masm=intel -nostdlib -ffreestanding -std=c++11 -mno-red-zone -fno-exceptions  -nostdlib -fno-rtti -Wall -Wextra
LDFLAGS := -T linkscript.ld

shell: main.cc lib.o entry.o
	$(cc) $(CFLAGS) entry.o main.cc lib.o -o shell $(LDFLAGS)

lib.o : lib.cc
	$(cc) $(CFLAGS) -c lib.cc -o lib.o 

entry.o : entry.S 
	$(cc) $(CFLAGS) -c entry.S -o entry.o

clean:
	rm *.o