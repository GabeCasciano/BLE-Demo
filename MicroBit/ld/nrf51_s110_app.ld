
/* micro:bit v1 (nRF51822) + SoftDevice S110 v8.0.0
   App at 0x0001B000, RAM from 0x20002000 */
MEMORY {
  FLASH (rx) : ORIGIN = 0x0001B000, LENGTH = 0x40000 - 0x1B000
  RAM   (rwx): ORIGIN = 0x20002000, LENGTH = 0x4000  - 0x2000
}
ENTRY(Reset_Handler)

SECTIONS {
  /* Vector table */
  .isr_vector ORIGIN(FLASH) :
  {
    KEEP(*(.isr_vector))
    KEEP(*(.vectors))
    KEEP(*(.gcc.vectorTable))
  } > FLASH

  /* Code + const */
  .text : { *(.text*) *(.rodata*) *(.glue_7) *(.glue_7t) } > FLASH
  .ARM.exidx : { *(.ARM.exidx*) } > FLASH

  /* Initialized data */
  .data : AT (LOADADDR(.text) + SIZEOF(.text)) {
    *(.data*)
  } > RAM

  /* Zero-init data */
  .bss (NOLOAD) : {
    *(.bss*) *(COMMON)
  } > RAM

  /* Top of stack */
  _estack = ORIGIN(RAM) + LENGTH(RAM);
  PROVIDE(__StackTop = _estack);
}
