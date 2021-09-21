/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

OUTPUT_ARCH( "riscv" )

ENTRY( boot )

MEMORY {
  lm (rxai!w) : ORIGIN = flash + 0x2000, LENGTH = 2K
  vm (rxai!w) : ORIGIN = flash + 0x2000, LENGTH = 2K
}

SECTIONS {

  .boot : ALIGN(32/8)
  {
    KEEP(*(.text.boot)); 
  } >vm AT>lm

}
