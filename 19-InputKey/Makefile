PROJ=example
obj-m := example.o

ifeq ($(KERNELDIR),)
KERNELDIR=/lib/modules/$(shell uname -r)/build
endif

all:
	make -C $(KERNELDIR) M=$(PWD) modules

test-pre:
	sudo insmod ./$(PROJ).ko
	dmesg | tail
	cat /proc/bus/input/devices
	cc test-application/main.c -o read_key 

test-action:
	dmesg | tail -n 40
	sudo ./read_key /dev/input/event5 &
	dmesg | tail
	sleep 5
	sudo killall read_key
test-post:
	sudo rmmod $(PROJ)
	dmesg | tail

test: test-pre test-action test-post

clean:
	make -C $(KERNELDIR) M=$(PWD) clean
	rm -f read_key
