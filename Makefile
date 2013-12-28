HEADERS:= idt.h kutil.h screen.h
all: kernel bootloader img
bootloader: bootloader.s
	nasm bootloader.s
kernel.o: kernel.c
	gcc -ffreestanding -c kernel.c
kutil.o: kutil.h kutil.c
	gcc -c kutil.c
util.o: util.s
	nasm -f elf util.s
screen.o: screen.c screen.h
	gcc -c screen.c
kernel: kutil.o util.o kernel.o screen.o lds $(HEADERS)
	ld -T lds kernel.o kutil.o util.o screen.o -o kernel
img: Makefile bootloader kernel
	dd if=/dev/zero of=img ibs=1 count=5120
	dd if=bootloader of=img ibs=1 conv=notrunc
	dd if=kernel of=img ibs=1 obs=1 seek=512 conv=notrunc

clean:
	rm -f bootloader kernel kernel.o img kutil.o
