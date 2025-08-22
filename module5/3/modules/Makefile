obj-m += blink_module.o

module:=blink_module

all: build_mod insert_mod

insert_mod:
	sudo insmod $(module).ko

build_mod:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	