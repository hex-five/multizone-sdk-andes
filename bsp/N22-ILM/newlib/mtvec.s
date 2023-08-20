/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

        .align 2

_mtvec:

irq0:   .word trp_isr
irq1:   .word 1f
irq2:   .word 1f
irq3:   .word msi_isr
irq4:   .word 1f
irq5:   .word 1f
irq6:   .word 1f
irq7:   .word tmr_isr
irq8:   .word 1f
irq9:   .word 1f
irq10:  .word 1f
irq11:  .word 1f
irq12:  .word 1f
irq13:  .word 1f
irq14:  .word 1f
irq15:  .word 1f
irq16:  .word 1f
irq17:  .word 1f
irq18:  .word 1f
irq19:  .word 1f
irq20:  .word 1f
irq21:  .word 1f
irq22:  .word 1f
irq23:  .word 1f
irq24:  .word 1f
irq25:  .word gpio_isr
irq26:  .word 1f
irq27:  .word uart_isr
irq28:  .word dma_isr
irq29:  .word 1f
irq30:  .word 1f
irq31:  .word 1f

        .weak trp_isr, msi_isr, tmr_isr, dma_isr, uart_isr, gpio_isr

trp_isr:
msi_isr:
tmr_isr:
dma_isr:
gpio_isr:
1:      j .
