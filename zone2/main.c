/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#include <string.h>

#include "platform.h"
#include "multizone.h"


static volatile int led = LED1;
static volatile char msg[16] = {'\0'};

__attribute__((interrupt())) void trp_handler(void)	 { // trap handler (0)

	const unsigned long mcause = MZONE_CSRR(CSR_MCAUSE);

	switch (mcause) {
	case 0:	break; // Instruction address misaligned
	case 1:	break; // Instruction access fault
	case 3:	break; // Breakpoint
	case 4:	break; // Load address misaligned
	case 5:	break; // Load access fault
	case 6:	break; // Store/AMO address misaligned
	case 7:	break; // Store access fault
	case 8:	break; // Environment call from U-mode
	}

	for( ;; );

}
__attribute__((interrupt())) void msi_handler(void)  { // machine software interrupt (3)

	char const tmp[16];

	if (MZONE_RECV(1, tmp))
		memcpy((char *)msg, tmp, sizeof msg);

}
__attribute__((interrupt())) void tmr_handler(void)  { // machine timer interrupt (7)

	// togle led
	BITINV(GPIO_BASE+GPIO_OUTPUT_VAL, led);

	// set timer (clears mip)
	MZONE_ADTIMECMP((uint64_t)(RTC_FREQ/1000*1000));

}
__attribute__((interrupt())) void gpio_handler(void) { // local interrupt (25)

	// GPIO: read int
	const uint32_t gpio_int = GPIO_REG(GPIO_INT_STATUS);

	// Switch LED based on button pressed
	BITCLR(GPIO_BASE+GPIO_OUTPUT_VAL, LED1 | LED2 | LED3 | LED4);
	switch (gpio_int) {
		case BTN1: led = LED1; MZONE_SEND(1, (char [16]){"IRQ 25 BTN 1"}); break;
		case BTN2: led = LED2; MZONE_SEND(1, (char [16]){"IRQ 25 BTN 2"}); break;
		case BTN3: led = LED3; MZONE_SEND(1, (char [16]){"IRQ 25 BTN 3"}); break;
		case BTN4: led = LED4; MZONE_SEND(1, (char [16]){"IRQ 25 BTN 4"}); break;
	}

	// GPIO: clear int
	GPIO_REG(GPIO_INT_STATUS) = gpio_int;
}

int main (void){

	//while(1) MZONE_WFI();
	//while(1) MZONE_YIELD();
	//while(1);

	// vectored trap handler
	static void (*trap_vect[32])(void) = {};
	trap_vect[0] = trp_handler;
	trap_vect[3] = msi_handler;
	trap_vect[7] = tmr_handler;
	trap_vect[25] = gpio_handler;
	CSRW(mtvec, trap_vect);
	CSRS(mtvec, 0x1);

	// GPIO: enable interrupts
	GPIO_REG(GPIO_INT_STATUS) = 0xFFFFFFFF;
	GPIO_REG(GPIO_INT_EN) |= ( BTN1 | BTN2 | BTN3 | BTN4 );

	// GPIO: set trigger mode to Negative-edge 0x5
	GPIO_REG(GPIO_INT_MODE) |= (0x5<<4 | 0x5<<8 | 0x5<<12 | 0x5<<16);

	// GPIO: set debounce
	GPIO_REG(GPIO_DEBOUNCE_EN) |= ( BTN1 | BTN2 | BTN3 | BTN4 );
	GPIO_REG(GPIO_DEBOUNCE_CTRL) = 0xFF; // 30us x (256+1) = 7.710ms

	// GPIO: enable LED outputs
	GPIO_REG(GPIO_OUTPUT_EN) |= ( LED1 | LED2 | LED3 | LED4);

	// GPIO: enable local interrupt
	CLIC_REG(CLIC_INT_PENDING+(CLIC_SRC_GPIO<<CLIC_SHIFT_PER_SRC)) |= 0x100;

    // enable and set timer
	MZONE_ADTIMECMP((uint64_t)(RTC_FREQ/1000*1000));
	CSRS(mie, 1<<7);

    // enable msip/inbox interrupt
	CSRS(mie, 1<<3);

	// enable global interrupts
	CSRS(mstatus, 1<<3);

	while(1){

		// Message handler
		CSRC(mie, 1<<3);

			if (msg[0] != '\0'){

				if (strncmp("ping", (char *)msg, sizeof msg[0]) == 0)
					MZONE_SEND(1, (char[16]){"pong"});
				else if (strcmp("mie=0", (char *)msg) == 0)
					CSRC(mstatus, 1 << 3);
				else if (strcmp("mie=1", (char *)msg) == 0)
					CSRS(mstatus, 1 << 3);
				else if (strcmp("block", (char *)msg) == 0) {
					CSRC(mstatus, 1 << 3);
					for ( ;; );
				} else
					MZONE_SEND(1, msg);

				msg[0] = '\0';

			}

		CSRS(mie, 1<<3);

		// Test workload ~4ms @20MHz
		// for(volatile int i=0; i<10000; i++){;}

		// Wait For Interrupt
		MZONE_WFI();

	}

}
