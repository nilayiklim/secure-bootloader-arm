MEMORY {
    FLASH (rx) : ORIGIN = 0x00008000, LENGTH = 224K
    RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 64K
}

SECTIONS {
    .text : {
        KEEP(*(.isr_vector))
        *(.text*)
        *(.rodata*)
    } > FLASH

    .data : { *(.data*) } > RAM AT > FLASH
    .bss  : { *(.bss*)  } > RAM
}