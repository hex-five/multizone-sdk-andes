/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include <platform.h>

/* https://www.embecosm.com/appnotes/ean9/ean9-howto-newlib-1.0.html */

// ----------------------------------------------------------------------------
int _close(int file) {
// ----------------------------------------------------------------------------

	return -1;
}

// ----------------------------------------------------------------------------
int _fstat(int file, struct stat *st) {
// ----------------------------------------------------------------------------

	st->st_mode = S_IFCHR;
	return 0;
}

// ----------------------------------------------------------------------------
void * _sbrk(int incr) {
// ----------------------------------------------------------------------------

	extern char _end[];
	extern char _heap_end[];
	static char *_heap_ptr = _end;

	if ((_heap_ptr + incr < _end) || (_heap_ptr + incr > _heap_end))
		return  (void *) -1;

	_heap_ptr += incr;
	return _heap_ptr - incr;
}

// ----------------------------------------------------------------------------
int _isatty(int file) {
// ----------------------------------------------------------------------------

	return (file == STDIN_FILENO || file == STDOUT_FILENO || file == STDERR_FILENO) ? 1 : 0;

}

// ----------------------------------------------------------------------------
int _lseek(int file, off_t ptr, int dir) {
// ----------------------------------------------------------------------------

	return 0;
}

// ----------------------------------------------------------------------------
int _open(const char* name, int flags, int mode) {
// ----------------------------------------------------------------------------

	if (strcmp(name, "UART")==0){ // 115200/8/N/1

	  // Step 1 Set baud rate

		// 1. Set the DLAB bit of the Line Control Register to 1.
		//lw a0, LCR(x2); ori a0, a0, DLAB; sw a0, LCR(x2);
		UART_REG(UART_LCR) |= UART_LCR_DLAB;

		// 2. Set DLL and DLM registers to select the desired baud rate for the UART.
		//   The divisor value = the frequency of uclk / (desired baud rate x OSCR)
		UART_REG(UART_DLM) = 0x0; UART_REG(UART_DLL) = 11; // 22 = 40M/115200/16

		// 3. Set the DLAB bit of the Line Control Register back to 0.
		UART_REG(UART_LCR) &= ~(UART_LCR_DLAB);

	  // Step 2 Set the Line Control Register

	 	// 1. Disable the parity: Set PEN = 0;
		UART_REG(UART_LCR) &= ~(UART_LCR_PEN);

		// 2. Set STOP bits to 1: Set STB = 0;
		UART_REG(UART_LCR) &= ~(UART_LCR_STB);

		// 3. Set word length to 8-bit: Set WLS = 3.
		UART_REG(UART_LCR) |= UART_LCR_WLS;

	  // Step 3 Set FIFOE in the FIFO Control Register to enable the FIFO.
		UART_REG(UART_FIFOCR) |= UART_FIFOCR_FIFOE;

	  // Enable RX interrupt
		UART_REG(UART_IE) |= UART_IE_ERBI;

		return 0;

	}

	return -1;
}

// ----------------------------------------------------------------------------
int _read(int file, char *ptr, size_t len) {
// ----------------------------------------------------------------------------

	if (isatty(file)) {

		size_t count = 0;

		while( count<len && (UART_REG(UART_LSR) & UART_LSR_DR)){

			*ptr++ = (char) (UART_REG(UART_RBR));
			count++;
		}

		return count;
	}

	return -1;
}

// ----------------------------------------------------------------------------
size_t _write(int file, const void *ptr, size_t len) {
// ----------------------------------------------------------------------------

	if (isatty(file)) {

		const uint8_t * buff = (uint8_t *)ptr;

		for (size_t i = 0; i < len; i++) {

			while ( (UART_REG(UART_LSR) & UART_LSR_THRE) == 0){;}

			UART_REG(UART_THR) = buff[i];

			if (buff[i] == '\n') {
				while ( (UART_REG(UART_LSR) & UART_LSR_THRE) == 0){;}
				UART_REG(UART_THR) = '\r';
			}
		}

		return len;

	}

	return -1;
}

// ----------------------------------------------------------------------------
int _kill (int  pid, int  sig) {
// ----------------------------------------------------------------------------

	return -1;

}

// ----------------------------------------------------------------------------
int _getpid () {
// ----------------------------------------------------------------------------

	return  1;

}

// open("UART", 0, 0); write(1, "Ok >\n", 5); char c='\0'; while(1) if ( read(0, &c, 1) >0 ) write(1, &c, 1);
