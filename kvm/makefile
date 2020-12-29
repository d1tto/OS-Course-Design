GXX := g++
CFLAGS := -g -std=c++11 -no-pie 

vm : main.cc kvm.o helper.o
	$(GXX) $(CFLAGS) main.cc kvm.o helper.o -o vm

helper.o : helper.cc 
	$(GXX) $(CFLAGS) -c helper.cc -o helper.o

kvm.o : kvm.cc 
	$(GXX) $(CFLAGS) -c kvm.cc -o kvm.o

clean:
	rm *.o