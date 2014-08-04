/******************************************************************************
* Serial Peripheral Interface Module                                          *
*                                                                             *
* Contains functions and definitions required for 2014 ROV motherboard        *
*                                                                             *
* (void) spiInit(void)                  Function initializes SPI system into  *
*                                       0,0 (clock idles low), LSB first      *
*                                       master mode at 1Mhz.                  *
*                                       will disable all peripherals.         *
* (uint8_t) spiXfr(uint8_t)             Function exchanges byte, returns      *
*                                       value left in SPI Rx buffer. Blocks   *
*                                       until Tx complete.                    *
*                                                                             *
******************************************************************************/

extern void spiInit(void);
extern uint8_t spiXfr(uint8_t byte);
