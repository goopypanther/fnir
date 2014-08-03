# AVR-GCC Makefile
#---Prefs---
PROJECT=fnir
MMCU=m8u2
F_CPU=16000000 # 16 MHz

LFUSEBITS=0xFF
HFUSEBITS=0xD9
EFUSEBITS=0xF4
#-----------



SOURCES=main.c spi.c 2494_adc.c
CC=avr-gcc
OBJCOPY=avr-objcopy

CFLAGS=-g -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wstrict-prototypes -DF_CPU=$(F_CPU) -Wa,-adhlns=$(<:.c=.lst) -I./ -mmcu=$(MMCU) -Wall
AVRDUDEFLAGS=-p $(MMCU)

ALL: $(PROJECT).hex filesize

$(PROJECT).hex: $(PROJECT).elf
	avr-objcopy -j .text -j .data -O ihex $(PROJECT).elf $(PROJECT).hex

$(PROJECT).elf: $(SOURCES)
	$(CC) $(CFLAGS) -o $(PROJECT).elf $(SOURCES)

program-isp: $(PROJECT).hex
	avrdude $(AVRDUDEFLAGS) -c usbasp -F -e -U flash:w:$(PROJECT).hex

program: $(PROJECT).hex
	avrdude $(AVRDUDEFLAGS) -c stk500v1 -P /dev/ttyUSB0 -b 19200 -D -U flash:w:$(PROJECT).hex:i

burn-fuse:
	avrdude $(AVRDUDEFLAGS) -c usbasp -F -u -U lfuse:w:$(LFUSEBITS):m -U hfuse:w:$(HFUSEBITS):m -U efuse:w:$(EFUSEBITS):m

# Display size of file.
filesize:
	avr-size -C --mcu=$(MMCU) $(PROJECT).elf 

clean:
	rm -f $(PROJECT).elf $(PROJECT).hex *.lst
