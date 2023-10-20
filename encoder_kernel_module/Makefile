# compiles the kernel module mydev.c
# To install kernel module run the following in terminal:
# sudo insmod mydev.ko
obj-m += kernel_module_poll.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules


