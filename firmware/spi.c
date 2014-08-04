#include "includes.h"

void spiInit(void) {
    
    SPCR |= ((1<<SPE) | (1<<DORD) | (1<<MSTR) | (1<<SPR0)); // Enable, LSB first, master, 16x prescaler

    // Flush SPI buffers
    (void) spiXfr(0x00);
    (void) spiXfr(0x00);
    (void) spiXfr(0x00);
}

uint8_t spiXfr(uint8_t byte) {
    SPDR = byte; // Start SPI Tx

    while (!(SPSR & (1<<SPIF))) {} // Wait until SPI transmission is finished

    return (SPDR);
}
