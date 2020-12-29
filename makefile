all:
	cd kvm && make
	cd kernel && make
	cd user && make
	cp kvm/vm kernel/kernel.bin user/shell .

clean:
	cd kvm && make clean
	cd kernel && make clean
	cd user && make clean