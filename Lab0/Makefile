toolchain = aarch64-linux-gnu-
CC = $(toolchain)gcc
LD = $(toolchain)ld
OBJCOPY = $(toolchain)objcopy

CFLAGA = -Wall -O3

.PHONY: all clean run

all : kernel8.img

main.o: main.s
	$(CC) $(CFLAGA) -c main.s -o main.o

kernel8.img: main.o
	$(LD) -T linker.ld -o kernel8.elf main.o
	$(OBJCOPY) -O binary kernel8.elf kernel8.img

clean:
	rm -rf *.o

run:
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -display none -d in_asm

