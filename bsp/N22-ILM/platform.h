/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#ifndef HEXFIVE_PLATFORM_H
#define HEXFIVE_PLATFORM_H

#define CPU_FREQ	20000000
#define RTC_FREQ 	20000000

// -----------------------------------------------------------------------------
// RTC (CLINT)
// -----------------------------------------------------------------------------
#define CLINT_BASE	0xE6000000
#define CLINT_MTIMECMP  0x0008
#define CLINT_MTIME	    0x0000

// -----------------------------------------------------------------------------
// CLIC
// -----------------------------------------------------------------------------
#define CLIC_BASE 	0xE2000000
#define CLIC_INT_THRESH 	   0x8	// 16.9.3 CLIC Interrupt Level Thresh   0x0008 Extent 4B
#define CLIC_INT_PENDING	0x1000	// 16.9.4 CLIC Interrupt Pending    0x1000+4*i Extent 1B
#define CLIC_INT_ENABLE		0x1001	// 16.9.5 CLIC Interrupt Enable     0x1001+4*i Extent 1B
#define CLIC_INT_ATTR		0x1002	// 16.9.6 CLIC Interrupt Attribute  0x1002+4*i Extent 1B
#define CLIC_INT_CTRL		0x1003	// 16.9.7 CLIC Interrupt Input Crtl 0x1003+4*i Extent 1B

#define CLIC_SHIFT_PER_SRC 		 2

#define CLIC_SRC_GPIO	25
#define CLIC_SRC_UART	27
#define CLIC_SRC_DMA	28

// -----------------------------------------------------------------------------
// DMA (single channel)
// ------------------------------------------------------------------------------
#define DMA_BASE 		0xE0E00000

#define DMA_VER			0x00
#define DMA_CFG			0x10
#define DMA_CTRL		0x20
#define DMA_CH_STATUS	0x30 /* TC: 1<<ch+16, AB: 1<<ch+8,  ERR: 1<<ch+0 */
#define DMA_CH_ENABLE	0x34 /* 1<<ch */
#define DMA_CH_ABORT	0x40 /* 1<<ch */
#define DMA_CH_CTRL		0x44 /* +ch*0x14 */
#define DMA_TR_SRC		0x48 /* +ch*0x14 */
#define DMA_TR_DEST		0x4C /* +ch*0x14 */
#define DMA_TR_SIZE		0x50 /* +ch*0x14 */

// -----------------------------------------------------------------------------
// UART2 - Andes ATCUART100
// -----------------------------------------------------------------------------
#define UART_BASE 	  		0xF0300000
#define UART_LCR 			0x2C	// Line Control Register (0x2C)
#define UART_LCR_WLS		0b11	// LCR.WLS 1:0 Word length setting, 0b11 = 8-bit
#define UART_LCR_STB		1<<2	// LCR.STB Number of STOP bits, 0 = 1 bits
#define UART_LCR_PEN		1<<3	// LCR.PEN Parity enable bit
#define UART_LCR_DLAB		1<<7	// LCR.DLAB Divisor latch access bit
#define UART_DLL			0x20	// Divisor Latch LSB (when DLAB = 1) (0x20)
#define UART_DLM			0x24	// Divisor Latch MSB (when DLAB = 1) (0x24)
#define UART_IE				0x24	// Interrupt Enable Register (when DLAB = 0) (0x24)
#define UART_IE_ERBI 		1<<0	// Enable received data available interrupt
#define UART_FIFOCR			0x28	// FIFO Control Register (0x28)
#define UART_FIFOCR_FIFOE	1<<0	// FIFO enable
#define UART_LSR			0x34	// Line Status Register (0x34)
#define UART_LSR_THRE		1<<5	// THRE Transmitter Holding Register empty
#define UART_LSR_DR			1<<0	// DR Data ready
#define UART_THR 			0x20	// Transmitter Holding Register (when DLAB = 0) (0x20)
#define UART_RBR 			0x20	// Receiver Buffer Register (when DLAB = 0) (0x20)

// -----------------------------------------------------------------------------
// GPIO - Andes ATCGPIO100
// -----------------------------------------------------------------------------
#define GPIO_BASE 			0xF0700000
#define GPIO_OUTPUT_EN  	0x28	// 3.2.5. Channel Direction Register (Offset 0x28)
#define GPIO_INPUT_VAL  	0x20	// 3.2.3. Channel Data-In Register   (Offset 0x20)
#define GPIO_OUTPUT_VAL 	0x24	// 3.2.4. Channel Data-Out Register  (Offset 0x24)
#define GPIO_INT_EN			0x50	// 3.2.10. Interrupt Enable Register (Offset 0x50)
#define GPIO_INT_MODE		0x54	// 3.2.11. Interrupt Mode Register (Offset 0x54, 0x58, 0x5C, 0x60)
#define GPIO_INT_STATUS		0x64	// 3.2.12. Interrupt Status Register (Offset 0x64)
#define GPIO_DEBOUNCE_EN 	0x70	// 3.2.13. De-bounce Enable Register (Offset 0x70)
#define GPIO_DEBOUNCE_CTRL 	0x74	// 3.2.14. De-bounce Control Register (Offset 0x74)

// -----------------------------------------------------------------------------
// SMU - Andes AE250 System Management Unit
// -----------------------------------------------------------------------------
#define SMU_BASE 			0xF0100000
#define SMU_PINMUX_CTRL0	0x1000	// PIN MUX control register 0 Section 15.10.11
#define SMU_PINMUX_CTRL1	0x1004	// PIN MUX control register 1 Section 15.10.12

// -----------------------------------------------------------------------------
// Buttons (GPIO IRQ)
// ------------------------------------------------------------------------------
#define BTN1 1<<1
#define BTN2 1<<2
#define BTN3 1<<3
#define BTN4 1<<4

// -----------------------------------------------------------------------------
// LEDs (GPIO)
// ------------------------------------------------------------------------------
#define LED1 1<<5
#define LED2 1<<6
#define LED3 1<<7
#define LED4 1<<8

// -----------------------------------------------------------------------------
// C Helper functions
// -----------------------------------------------------------------------------

#define _REG64(base, offset) (*(volatile uint64_t *)((base) + (offset)))
#define _REG32(base, offset) (*(volatile uint32_t *)((base) + (offset)))
#define _REG16(base, offset) (*(volatile uint16_t *)((base) + (offset)))

#define DMA_REG(offset) _REG32(DMA_BASE, offset)
#define CLIC_REG(offset) _REG32(CLIC_BASE, offset)
#define GPIO_REG(offset) _REG32(GPIO_BASE, offset)
#define UART_REG(offset) _REG32(UART_BASE, offset)
#define SMU_REG(offset) _REG32(SMU_BASE, offset)
#define RTC_REG(offset) _REG64(RTC_BASE, offset)

#endif /* HEXFIVE_PLATFORM_H */
