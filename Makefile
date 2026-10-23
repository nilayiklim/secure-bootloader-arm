CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -mcpu=cortex-m3 -mthumb -Os -Wall -nostdlib

all: bootloader.bin firmware_signed.bin

bootloader.bin:
	$(CC) $(CFLAGS) -T bootloader/bootloader.ld bootloader/src/startup.c bootloader/src/main.c bootloader/src/flash.c bootloader/src/uart.c crypto/sha256.c crypto/ecdsa.c -o bootloader.elf
	$(OBJCOPY) -O binary bootloader.elf bootloader.bin
	
firmware.bin:
	$(CC) $(CFLAGS) -T firmware_mock/firmware.ld firmware_mock/src/main.c -o firmware.elf
	$(OBJCOPY) -O binary firmware.elf firmware.bin

firmware_signed.bin: firmware.bin
	python3 tools/sign_firmware.py

clean:
	rm -f *.elf *.bin