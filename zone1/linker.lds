/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

OUTPUT_ARCH( "riscv" )

ENTRY( _start )

MEMORY {
  lm  (rxai!w) : ORIGIN = flash + 0x8000, LENGTH = 32K
  vm  (rxai!w) : ORIGIN = flash + 0x8000, LENGTH = 32K
  ram (wa!xri) : ORIGIN = dlm   + 0x6000, LENGTH =  8K
}

SECTIONS {

  __stack_size = DEFINED(__stack_size) ? __stack_size : 2K;
  PROVIDE(__stack_size = __stack_size);


  /** Init / Fini related code | run once - loaded in slower memory **/

  .init : ALIGN(32/8)
  {
    KEEP (*(SORT_NONE(.init)))
  } >vm AT>lm 

  .fini : ALIGN(32/8)
  {
    KEEP (*(SORT_NONE(.fini)))
  } >vm AT>lm 

  .preinit_array : ALIGN(32/8) 
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >vm AT>lm

  .init_array : ALIGN(32/8) 
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >vm AT>lm

  .fini_array : ALIGN(32/8)
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >vm AT>lm 

  .ctors : ALIGN(32/8)
  {
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  } >vm AT>lm 

  .dtors : ALIGN(32/8)
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } >vm AT>lm 


  /** Text section | vmram - can be loaded to faster memory **/
  
  .text : ALIGN(32/8) 
  { 
    *(.text.unlikely .text.*_unlikely .text.unlikely.*)
    *(.text.exit .text.exit.*)
    *(.text.startup .text.startup.*)
    *(.text.hot .text.hot.*)
    *(SORT(.text.sorted.*))
    *(.text .stub .text.* .gnu.linkonce.t.*)
    /* .gnu.warning sections are handled specially by elf.em.  */
    *(.gnu.warning)
  } >vm AT>lm 

  PROVIDE (__etext = .);
  PROVIDE (_etext = .);
  PROVIDE (etext = .);

  .rodata : ALIGN(32/8)
  {
    *(.rdata)
    *(.rodata .rodata.*)
    *(.gnu.linkonce.r.*)
  } >vm AT>lm 


  /* Data section - copied from rom to ram */

  .lalign : ALIGN(32/8) 
  {
    PROVIDE( _data_lma = . );
  } >vm AT>lm  

  .data : ALIGN(32/8)    
  {
    __DATA_BEGIN__ = .;
    _data = .;
    *(.data .data.* .gnu.linkonce.d.*)
  } >ram AT>lm

  .sdata : ALIGN(32/8)    
  {
    PROVIDE( __global_pointer$ = . + 0x800 );
    __SDATA_BEGIN__ = .;
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata .srodata.*)
    *(.sdata .sdata.* .sdata* .gnu.linkonce.s.*)
  } >ram AT>lm

  _edata = .; 
  PROVIDE (edata = .);

  .bss (NOLOAD): ALIGN(32/8)  
  {
    __bss_start = .;
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.bss .bss.* .gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(. != 0 ? 64 / 8 : 1);
  } >ram AT>ram 

  . = ALIGN(32/8);
  . = SEGMENT_START("ldata-segment", .);
  . = ALIGN(32/8);
  __bss_end = .;
  __BSS_END__ = .;


  /* Stack definition - change size at top of this script */

  .stack (NOLOAD): ALIGN(16)  
  {
    . += __stack_size;
    PROVIDE( _sp = . );
  } >ram AT>ram 


  /* Heap definition - calculated to use the remaining memory */

  .heap (NOLOAD): ALIGN(32/8)  
  {
    PROVIDE( _end = . );
    PROVIDE( __end = . );
    PROVIDE( __heap_start = . );
    . += LENGTH(ram) - ( . - ORIGIN(ram));
    PROVIDE( _heap_end = . );
    PROVIDE( __heap_end = . );
  } >ram AT>ram 

}