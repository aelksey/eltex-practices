obj-m +=chardev_module.o

module:=chardev_module

all: build_mod insert_mod run_test

insert_mod:
	sudo insmod $(module).ko

build_mod:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f test
	
test: test.c
	cc test.c -o test

run_test: test
	sudo ./test