/******************************************************************************
* 2494 ADC Communication Library                                              *
* By Jeremy Ruhland                                                           *
*                                                                             *
******************************************************************************/

typedef union {
    uint16_t bin;
    struct {
        uint8_t preamble : 2; // Preamble bits, identical for all messages
        uint8_t en       : 1; // Enable bit selects adc enable
        uint8_t sgl      : 1; // Bipolar/monopolar mode selection bit
        uint8_t odd      : 1; // Selects between even/odd bit number
        uint8_t a        : 3; // Address of channel selected for conversion
        uint8_t en2      : 1; // Second enable bit selected if settings have changed
        uint8_t im       : 1; // Selects internal/external voltage source
        uint8_t f        : 2; // Selects powerline frequency rejection mode
        uint8_t spd      : 1; // Selects conversion speed & auto calibration
        uint8_t gs       : 3; // Configures internal gain stage
    } bitfield;
} adcMemory_t;

void adcInit(void) {}

adcReturn_t adcSelect(adcState_t adcState,
                   adcChannelType_t adcChannelType,
                   adcRejectionMode_t adcRejectionMode,
                   adcSpeed_t adcSpeed,
                   adcGain_t adcGain) {

    adcMemory_t adcMemory;
    adcReturn_t adcReturn;
    uint8_t adcReturnBuffer[3];

    adcMemory.bitfield.preamble = 0x02; // All commands begin with 0b10

    // Set enable bits
    switch (adcState) {
        case DISABLE :
            adcMemory.bitfield.en = 0x00;
            break;
        case ENABLE :
            adcMemory.bitfield.en = 0x01;
            adcMemory.bitfield.en2 = 0x01;
            break;
        case REPEAT :
            adcMemory.bitfield.en = 0x01;
            adcMemory.bitfield.en2 = 0x00;
            break;
        default:
            break;
    }

    // Set bits to select which bipolar/monopolar channel to mux into adc
    switch (adcChannelType) {
        case BIPOLAR_CH_0_1 :
            adcMemory.bitfield.sgl = 0x00;
            adcMemory.bitfield.odd = 0x00;
            adcMemory.bitfield.a = 0x00;
            adcMemory.bitfield.im = 0x00;
            break;
        case BIPOLAR_CH_1_0 :
            adcMemory.bitfield.sgl = 0x00;
            adcMemory.bitfield.odd = 0x01;
            adcMemory.bitfield.a = 0x00;
            adcMemory.bitfield.im = 0x00;
            break;
        case BIPOLAR_CH_2_3 :
            adcMemory.bitfield.sgl = 0x00;
            adcMemory.bitfield.odd = 0x00;
            adcMemory.bitfield.a = 0x01;
            adcMemory.bitfield.im = 0x00;
            break;
        case BIPOLAR_CH_3_2 :
            adcMemory.bitfield.sgl = 0x00;
            adcMemory.bitfield.odd = 0x01;
            adcMemory.bitfield.a = 0x01;
            adcMemory.bitfield.im = 0x00;
            break;
        case BIPOLAR_CH_4_5 :
            adcMemory.bitfield.sgl = 0x00;
            adcMemory.bitfield.odd = 0x00;
            adcMemory.bitfield.a = 0x02;
            adcMemory.bitfield.im = 0x00;
            break;
        case BIPOLAR_CH_5_4 :
            adcMemory.bitfield.sgl = 0x00;
            adcMemory.bitfield.odd = 0x01;
            adcMemory.bitfield.a = 0x02;
            adcMemory.bitfield.im = 0x00;
            break;
        case BIPOLAR_CH_6_7 :
            adcMemory.bitfield.sgl = 0x00;
            adcMemory.bitfield.odd = 0x00;
            adcMemory.bitfield.a = 0x03;
            adcMemory.bitfield.im = 0x00;
            break;
        case BIPOLAR_CH_7_6 :
            adcMemory.bitfield.sgl = 0x00;
            adcMemory.bitfield.odd = 0x01;
            adcMemory.bitfield.a = 0x03;
            adcMemory.bitfield.im = 0x00;
            break;
        case BIPOLAR_CH_8_9 :
            adcMemory.bitfield.sgl = 0x00;
            adcMemory.bitfield.odd = 0x00;
            adcMemory.bitfield.a = 0x04;
            adcMemory.bitfield.im = 0x00;
            break;
        case BIPOLAR_CH_9_8 :
            adcMemory.bitfield.sgl = 0x00;
            adcMemory.bitfield.odd = 0x01;
            adcMemory.bitfield.a = 0x04;
            adcMemory.bitfield.im = 0x00;
            break;
        case BIPOLAR_CH_10_11 :
            adcMemory.bitfield.sgl = 0x00;
            adcMemory.bitfield.odd = 0x00;
            adcMemory.bitfield.a = 0x05;
            adcMemory.bitfield.im = 0x00;
            break;
        case BIPOLAR_CH_11_10 :
            adcMemory.bitfield.sgl = 0x00;
            adcMemory.bitfield.odd = 0x01;
            adcMemory.bitfield.a = 0x05;
            adcMemory.bitfield.im = 0x00;
            break;
        case BIPOLAR_CH_12_13 :
            adcMemory.bitfield.sgl = 0x00;
            adcMemory.bitfield.odd = 0x00;
            adcMemory.bitfield.a = 0x06;
            adcMemory.bitfield.im = 0x00;
            break;
        case BIPOLAR_CH_13_12 :
            adcMemory.bitfield.sgl = 0x00;
            adcMemory.bitfield.odd = 0x01;
            adcMemory.bitfield.a = 0x06;
            adcMemory.bitfield.im = 0x00;
            break;
        case BIPOLAR_CH_14_15 :
            adcMemory.bitfield.sgl = 0x00;
            adcMemory.bitfield.odd = 0x00;
            adcMemory.bitfield.a = 0x07;
            adcMemory.bitfield.im = 0x00;
            break;
        case BIPOLAR_CH_15_14 :
            adcMemory.bitfield.sgl = 0x00;
            adcMemory.bitfield.odd = 0x01;
            adcMemory.bitfield.a = 0x07;
            adcMemory.bitfield.im = 0x00;
            break;
        case MONOPOLAR_CH_0 :
            adcMemory.bitfield.sgl = 0x01;
            adcMemory.bitfield.odd = 0x00;
            adcMemory.bitfield.a = 0x00;
            adcMemory.bitfield.im = 0x00;
            break;
        case MONOPOLAR_CH_1 :
            adcMemory.bitfield.sgl = 0x01;
            adcMemory.bitfield.odd = 0x01;
            adcMemory.bitfield.a = 0x00;
            adcMemory.bitfield.im = 0x00;
            break;
        case MONOPOLAR_CH_2 :
            adcMemory.bitfield.sgl = 0x01;
            adcMemory.bitfield.odd = 0x00;
            adcMemory.bitfield.a = 0x01;
            adcMemory.bitfield.im = 0x00;
            break;
        case MONOPOLAR_CH_3 :
            adcMemory.bitfield.sgl = 0x01;
            adcMemory.bitfield.odd = 0x01;
            adcMemory.bitfield.a = 0x01;
            adcMemory.bitfield.im = 0x00;
            break;
        case MONOPOLAR_CH_4 :
            adcMemory.bitfield.sgl = 0x01;
            adcMemory.bitfield.odd = 0x00;
            adcMemory.bitfield.a = 0x02;
            adcMemory.bitfield.im = 0x00;
            break;
        case MONOPOLAR_CH_5 :
            adcMemory.bitfield.sgl = 0x01;
            adcMemory.bitfield.odd = 0x01;
            adcMemory.bitfield.a = 0x02;
            adcMemory.bitfield.im = 0x00;
            break;
        case MONOPOLAR_CH_6 :
            adcMemory.bitfield.sgl = 0x01;
            adcMemory.bitfield.odd = 0x00;
            adcMemory.bitfield.a = 0x03;
            adcMemory.bitfield.im = 0x00;
            break;
        case MONOPOLAR_CH_7 :
            adcMemory.bitfield.sgl = 0x01;
            adcMemory.bitfield.odd = 0x01;
            adcMemory.bitfield.a = 0x03;
            adcMemory.bitfield.im = 0x00;
            break;
        case MONOPOLAR_CH_8 :
            adcMemory.bitfield.sgl = 0x01;
            adcMemory.bitfield.odd = 0x00;
            adcMemory.bitfield.a = 0x04;
            adcMemory.bitfield.im = 0x00;
            break;
        case MONOPOLAR_CH_9 :
            adcMemory.bitfield.sgl = 0x01;
            adcMemory.bitfield.odd = 0x01;
            adcMemory.bitfield.a = 0x04;
            adcMemory.bitfield.im = 0x00;
            break;
        case MONOPOLAR_CH_10 :
            adcMemory.bitfield.sgl = 0x01;
            adcMemory.bitfield.odd = 0x00;
            adcMemory.bitfield.a = 0x05;
            adcMemory.bitfield.im = 0x00;
            break;
        case MONOPOLAR_CH_11 :
            adcMemory.bitfield.sgl = 0x01;
            adcMemory.bitfield.odd = 0x01;
            adcMemory.bitfield.a = 0x05;
            adcMemory.bitfield.im = 0x00;
            break;
        case MONOPOLAR_CH_12 :
            adcMemory.bitfield.sgl = 0x01;
            adcMemory.bitfield.odd = 0x00;
            adcMemory.bitfield.a = 0x06;
            adcMemory.bitfield.im = 0x00;
            break;
        case MONOPOLAR_CH_13 :
            adcMemory.bitfield.sgl = 0x01;
            adcMemory.bitfield.odd = 0x01;
            adcMemory.bitfield.a = 0x06;
            adcMemory.bitfield.im = 0x00;
            break;
        case MONOPOLAR_CH_14 :
            adcMemory.bitfield.sgl = 0x01;
            adcMemory.bitfield.odd = 0x00;
            adcMemory.bitfield.a = 0x07;
            adcMemory.bitfield.im = 0x00;
            break;
        case MONOPOLAR_CH_15 :
            adcMemory.bitfield.sgl = 0x01;
            adcMemory.bitfield.odd = 0x01;
            adcMemory.bitfield.a = 0x07;
            adcMemory.bitfield.im = 0x00;
            break;
        case INTERNAL_TEMP_CH :
            adcMemory.bitfield.im = 0x01;
            break;
        default :
            break;
    }

    // Set bits for powerline noise rejection
    switch (adcRejectionMode) {
        case REJECT_50HZ :
            adcMemory.bitfield.f = 0x00;
            break;
        case REJECT_60HZ :
            adcMemory.bitfield.f = 0x01;
            break;
        case REJECT_50HZ_60HZ :
            adcMemory.bitfield.f = 0x02;
            break;
        default :
            break;
    }

    // Set bits for adc conversion speed
    switch (adcSpeed) {
        case AUTO_CALIBRATE :
            adcMemory.bitfield.spd = 0x00;
            break;
        case DOUBLE_SPEED :
            adcMemory.bitfield.spd = 0x01;
            break;
        default :
            break;
    }

    // Set bits for internal adc gain
    switch (adcGain) {
        case GAIN_1X :
            adcMemory.bitfield.gain = 0x00;
            break;
        case GAIN_2X :
            adcMemory.bitfield.gain = 0x01;
            break;
        case GAIN_4X :
            adcMemory.bitfield.gain = 0x02;
            break;
        case GAIN_8X :
            adcMemory.bitfield.gain = 0x03;
            break;
        case GAIN_16X :
            adcMemory.bitfield.gain = 0x04;
            break;
        case GAIN_32X :
            adcMemory.bitfield.gain = 0x05;
            break;
        case GAIN_64X :
            adcMemory.bitfield.gain = 0x06;
            break;
        case GAIN_128X :
            adcMemory.bitfield.gain = 0x07;
            break;
        default :
            break;
    }

    // Put new command into adc, return result
    adcReturnBuffer[0] = spiXfr((uint8_t) (adcMemory.bin>>8));
    adcReturnBuffer[1] = spiXfr((uint8_t) adcMemory.bin);
    adcReturnBuffer[2] = spiXfr(0x00);

    if (adcReturnBuffer[0] & (1<<7)) {
        adcReturn.conversionStatus = ONGOING;
    } else {
        adcReturn.conversionStatus = FINISHED;
    }

    if (adcReturnBuffer[0] & (1<<5)) {
        adcReturn.returnSign = POSITIVE;
    } else {
        adcReturn.returnSign = NEGATIVE;
    }

    adcReturn.returnValue = (((uint16_t) adcReturnBuffer[0])<<11) & (((uint16_t) adcReturnBuffer[1])<<4) & (((uint16_t) adcReturnBuffer[2])>>4);

    return (adcReturn);
}
