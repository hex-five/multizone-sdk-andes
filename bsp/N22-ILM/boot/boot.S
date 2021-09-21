/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

/*
* Hardware setup / OEM low-level boot code
* Link to HEX format and pass to the multizone.jar via --boot option
* Runs in privileged machine mode "M" (after policy integrity check)
* Return execution to MultiZone via trap 0xb (ecall from M mode)
*/

#include "../platform.h"

.section .text.boot
.global boot

boot:

	# Reset DMA Controller
	la t0, DMA_BASE; li t1, 1; sw t1, DMA_CTRL(t0)

	# enable I-Cache
	csrsi 0x7ca, 0x1

	# exit to MultiZone
	ecall
