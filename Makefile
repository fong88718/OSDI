toolchain = aarch64-linux-gnu-
CC = $(toolchain)gcc
LD = $(toolchain)ld
OBJCOPY = $(toolchain)objcopy
obj = start.o main.o uart.o 


CFLAGS = -Wall -O3 -ffreestanding -nostdlib -nostartfiles

.PHONY: all clean run asm debug

all : kernel8.img

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

start.o: start.s
	$(CC) $(CFLAGS) -c start.s -o start.o

kernel8.img: $(obj)
	$(LD) -T linker.ld -o kernel8.elf $(obj)
	$(OBJCOPY) -O binary kernel8.elf kernel8.img

clean:
	rm -rf *.o *.img *.elf

asm:
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -display none -d in_asm

run:  
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -nographic -serial null -serial mon:stdio

debug:
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -display none -S -s

