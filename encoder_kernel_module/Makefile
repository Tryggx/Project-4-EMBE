# compiles the kernel module mydev.c
# To install kernel module run the following in terminal:
# sudo insmod mydev.ko
obj-m += mydev.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules


