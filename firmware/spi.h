/** @file spi.h
*
* @brief Serial Peripheral Interface Module.
* @author Jeremy Ruhland
* @date 8/2014
*
* Created for rov project but imported and edited for fNIR imager project.
*/

/** SPI initialization routine
*
* Initializes hardware SPI system and flushes buffers
*
* @return Function does not return a value
*/
extern void spiInit(void);

/** SPI data transfer
*
* Transfers one byte of data over spi system, function blocks until transfer is
* complete. You should call @ref spiInit before using this function.
*
* @param byte The byte intended to have transfered to the slave device
* @return Function returns one byte from slave device
*/
extern uint8_t spiXfr(uint8_t byte);
