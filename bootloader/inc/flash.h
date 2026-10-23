#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>

int flash_read(uint32_t address, uint8_t *buffer, uint32_t length);

uint32_t flash_get_firmware_size(void);

#endif 