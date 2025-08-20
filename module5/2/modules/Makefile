obj-m += proc_module.o

test_cc:=gcc
test_cc_flags:= -o
test_src:=test.c
test_exec:=test

module:=proc_module

all: build insert_mod run_test

insert_mod:
	sudo insmod $(module).ko

run_test:
	./$(test_exec)

build: build_module build_test

build_test:
	$(test_cc) $(test_src) $(test_cc_flags) $(test_exec)

build_module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f $(test_exec)
	