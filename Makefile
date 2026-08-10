toolchain = aarch64-linux-gnu-

CC := $(toolchain)gcc
LD := $(toolchain)ld
OBJCOPY := $(toolchain)objcopy

LINKER_FILE := src/linker.ld
MAKEFILE := Makefile

SRC_DIR := src
OUT_DIR := out

LINKER_FILE := $(SRC_DIR)/linker.ld

ASMS := $(wildcard $(SRC_DIR)/*.S)
ASM_OBJS := $(ASMS:$(SRC_DIR)/%.S=$(OUT_DIR)/%.o)

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OUT_DIR)/%.o) 

CFLAGS := -g -Wall -O0 -ffreestanding -nostdlib -nostartfiles -I include -mstrict-align

.PHONY: all clean asm run display debug tty print

all : kernel8.img

$(OUT_DIR)/%.o: $(SRC_DIR)/%.c | $(OUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT_DIR)/%.o: $(SRC_DIR)/%.S | $(OUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT_DIR)/%.o: $(SRC_DIR)/%.s  | $(OUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

kernel8.img:  $(OBJS) $(ASM_OBJS) $(LINKER_FILE) $(MAKEFILE)
	$(LD) -T $(LINKER_FILE) -o kernel8.elf $(ASM_OBJS) $(OBJS) 
	$(OBJCOPY) -O binary kernel8.elf kernel8.img
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

clean:
	rm -rf $(OUT_DIR) kernel8.*

asm: all
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -display none -d in_asm

run: all
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -display none -serial stdio

display: all
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial stdio

debug: all
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -display none -serial pty -S -s

tty: all
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial pty

print:
	
