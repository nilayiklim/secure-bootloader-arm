#include <stdint.h>
#include "../inc/memory_map.h"

int flash_read(uint32_t address, uint8_t *buffer, uint32_t length) {
    if (address < FLASH_BASE || (address + length) > (FLASH_BASE + BOOTLOADER_SIZE + FIRMWARE_MAX_SIZE)) {
        return -1;
    }

    uint8_t *flash_ptr = (uint8_t *)address;

    for (uint32_t i = 0; i < length; i++) {
        buffer[i] = flash_ptr[i];
    }

    return 0;
}

uint32_t flash_get_firmware_size(void) {
    return 1024;
}