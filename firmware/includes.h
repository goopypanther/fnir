// AVR-GCC include files
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Custom project specific include files
#include "spi.h"
#include "2494_adc.h"

// LUFA includes & defines
#include "Descriptors.h"
#include <LUFA/Drivers/Peripheral/Serial.h>
#include <LUFA/Drivers/USB/USB.h>
