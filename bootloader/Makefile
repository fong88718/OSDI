toolchain = aarch64-linux-gnu-

CC := $(toolchain)gcc
LD := $(toolchain)ld
OBJCOPY := $(toolchain)objcopy

SRC_DIR := src
OUT_DIR := out

LINKER_FILE := $(SRC_DIR)/linker.ld

ENTRY := $(SRC_DIR)/start.s
ENTRY_OBJ := $(OUT_DIR)/start.o

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OUT_DIR)/%.o) 

CFLAGS := -g -Wall -O0 -ffreestanding -nostdlib -nostartfiles -I include  -mstrict-align

.PHONY: all clean run asm debug print

all : kernel8.img

$(OUT_DIR)/%.o: $(SRC_DIR)/%.c | $(OUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(ENTRY_OBJ): $(ENTRY) | $(OUT_DIR)
	$(CC) $(CFLAGS) -c $(ENTRY) -o $(ENTRY_OBJ)

kernel8.img: $(OBJS) $(ENTRY_OBJ)
	$(LD) -T $(LINKER_FILE) -o kernel8.elf $(ENTRY_OBJ) $(OBJS) 
	$(OBJCOPY) -O binary kernel8.elf kernel8.img
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

clean:
	rm -rf $(OUT_DIR) kernel8.*

asm: all
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -display none -d in_asm

run: all
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -display none -serial stdio

debug: all
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -display none -serial stdio -S -s

tty: all
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial pty

print:
	
