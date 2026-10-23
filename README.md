# Secure Bootloader for ARM Cortex-M3 (Bare-Metal)

This repository contains a bare-metal secure bootloader developed for the ARM Cortex-M3 architecture. My primary goal with this project was to implement a robust, low-level startup sequence and cryptographic firmware verification entirely from scratch, bypassing heavy HAL libraries or RTOS dependencies.

By managing memory mapping via custom linker scripts and handling hardware registers directly, this project demonstrates a deep dive into embedded systems architecture and secure boot concepts.

## Boot Sequence & Architecture
The bootloader operates through a strict, secure pipeline:
1. **Hardware Initialization:** Boots from `0x00000000`, setting up the initial Stack Pointer and Vector Table.
2. **Cryptographic Verification:** Reads the incoming firmware payload and computes its SHA-256 hash. The hash is then verified against an embedded ECDSA public key.
3. **Safe Execution Handoff:** If the signature is valid, the bootloader safely relocates the Vector Table Offset Register (VTOR), updates the Main Stack Pointer (MSP), and performs a bare-metal branch to the application firmware.
4. **Halt on Failure:** If verification fails, the system halts securely in an infinite loop to prevent unauthorized code execution.

## Technical Highlights
- **Pure Bare-Metal C & Assembly:** Direct memory manipulation and pointer arithmetic without external abstractions.
- **Custom Linker Scripts (`.ld`):** Precise definition of ROM/RAM boundaries to enforce strict memory separation between the bootloader and the application space.
- **QEMU Emulation Ready:** Fully configured for the `lm3s6965evb` target, allowing rapid testing and simulation without physical hardware.

## Memory Layout
| Region | Base Address | Size | Description |
| :--- | :--- | :--- | :--- |
| **FLASH** | `0x00000000` | 256 KB | Total Flash Memory |
| **RAM**   | `0x20000000` | 64 KB  | Total SRAM |
| **Bootloader** | `0x00000000` | 32 KB | Secure Bootloader Execution Area |
| **Firmware**   | `0x00008000` | 224 KB | Signed Application (Mock Firmware) |

## Build & Simulation
This project is built using the standard GNU Arm Embedded Toolchain.

**1. Compile the bootloader, mock app, and sign the firmware:**
```bash
make clean && make
```

**2. Run the QEMU simulation:**
```bash
qemu-system-arm -M lm3s6965evb -cpu cortex-m3 -nographic -kernel bootloader.elf -device loader,file=firmware_signed.bin,addr=0x00008000
```

*(To exit the QEMU terminal simulation, press Ctrl + A, then X.)*