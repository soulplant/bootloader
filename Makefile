all: kernel bootloader img
bootloader: bootloader.s
	nasm bootloader.s
kernel.o: kernel.c
	gcc -ffreestanding -c kernel.c
kernel: kernel.o lds
	ld -T lds kernel.o -o kernel
img: bootloader kernel
	dd if=/dev/zero of=img ibs=1 count=1024
	dd if=bootloader of=img ibs=1 conv=notrunc
	dd if=kernel of=img ibs=1 obs=1 seek=512 conv=notrunc

clean:
	rm -f bootloader kernel kernel.o img
