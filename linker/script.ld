ENTRY(entry)

MEMORY
{
  ROM (rx)          : ORIGIN = 0x08000000, LENGTH = 512K
  RAM (rwx)         : ORIGIN = 0x20000000, LENGTH = 96K
}

REGION_ALIAS("heapmem", RAM);


SECTIONS
{
  .text :
  {
    KEEP(*(.vectors))
    *(.text*)
    . = ALIGN(4);
    *(.rodata*)
    . = ALIGN(4);
  } >ROM

  . = ALIGN(4);
  _etext = .;

  .data :
  {
    _data = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } >RAM AT> ROM
  _data_loadaddr = LOADADDR(.data);

  .bss :
  {
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > RAM

  /DISCARD/ : { *(.eh_frame) }

  .heap (NOLOAD) :
  {
    *(.heap*)
    _heap = .;
  } >heapmem

  . = ALIGN(4);
  end = .;
}

PROVIDE(_stack = ORIGIN(RAM) + LENGTH(RAM));
