#include "../inc/uart.h"
#include <stdint.h>
#include <stddef.h>
#include "../inc/memory_map.h"
#include "../inc/flash.h"
#include "../../crypto/sha256.h"
#include "../../crypto/ecdsa.h"

#define SCB_VTOR (*(volatile uint32_t *)0xE000ED08)
#define CHUNK_SIZE 256 

typedef void (*app_entry_t)(void);

static const uint8_t public_key[ECDSA_PUB_KEY_SIZE] = {0}; 

static int verify_firmware_signature(void) {
    uint32_t fw_size = flash_get_firmware_size();
    uint8_t buffer[CHUNK_SIZE];
    uint8_t computed_hash[SHA256_HASH_SIZE];
    uint8_t signature[ECDSA_SIGNATURE_SIZE];
    sha256_context_t ctx;

    sha256_init(&ctx);

    uint32_t offset = 0;
    while (offset < fw_size) {
        uint32_t read_size = (fw_size - offset > CHUNK_SIZE) ? CHUNK_SIZE : (fw_size - offset);
        
        if (flash_read(FIRMWARE_BASE + offset, buffer, read_size) != 0) {
            return 0; 
        }
        
        sha256_update(&ctx, buffer, read_size);
        offset += read_size;
    }

    sha256_final(&ctx, computed_hash);

    if (flash_read(FIRMWARE_BASE + fw_size, signature, ECDSA_SIGNATURE_SIZE) != 0) {
        return 0;
    }

    return ecdsa_verify(computed_hash, signature, public_key);
}

static void jump_to_app(void) {
    uint32_t *app_vector_table = (uint32_t *)FIRMWARE_BASE;
    uint32_t app_msp = app_vector_table[0];
    uint32_t app_reset_handler = app_vector_table[1];
    
    if (app_msp == 0xFFFFFFFF) {
        while(1);
    }

    SCB_VTOR = FIRMWARE_BASE;
    __asm volatile("msr msp, %0" :: "r" (app_msp));

    app_entry_t app = (app_entry_t)app_reset_handler;
    app();
}

int main(void) {
    uart_print("\n===================================\n");
    uart_print("[BOOTLOADER] System Booting...\n");
    uart_print("[BOOTLOADER] Verifying ECDSA Firmware Signature...\n");

    if (verify_firmware_signature()) {
        uart_print("[BOOTLOADER] Signature VALID! Access Granted.\n");
        uart_print("[BOOTLOADER] Relocating Vector Table and Jumping to Application...\n");
        uart_print("===================================\n\n");
        jump_to_app();
    }

    uart_print("[SECURITY ALERT] Signature INVALID! Boot sequence halted.\n");
    while(1) {
        
    }
    
    return 0;
}