/** @file main.c
* @brief fNIR Brain Imager
* @author Jeremy Ruhland
* @date 8/2014
*/

#include "includes.h"

// Private typedefs
/** USB system state enum */
typedef enum {USB_CONNECTED, /**< Usb system connected */
              USB_IDLE /**< Usb system disconnected */
} USB_sys_state_t;

/** fNIR scanning mode enum */
typedef enum {FNIR_NULL, /**< System is taking null offset measurement */
              FNIR_730NM, /**< System is taking 730nm measurement */
              FNIR_850NM, /**< System is taking 850nm measurement */
              FNIR_IDLE, /**< System is not currently taking any measurement */
              FNIR_STOP /**< System is paused and will not take measurements */
} fnir_mode_state_t;

// Private define macros
#define LED_ON() PORTB |= (1<<PB7)
#define LED_OFF() PORTB &= ~(1<<PB7)
#define LED_TOGGLE() PORTB ^= (1<<PB7)
#define CHIP_SELECT() PORTB |= (1<<PB6)
#define CHIP_DESELECT() PORTB &= ~(1<<PB6)

// Function prototypes
void mainLedInit(void);
void mainParseCommand(uint8_t receivedByte);
void mainFnirScan(void);
void mainNirLedControl(fnir_mode_state_t fnirMode, uint8_t channel);
adcReturn_t mainTakeMeasurement(uint8_t channel);
void mainReportResult(uint8_t channel, adcReturn_t *adcReturnValue);
void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);

// Global variables
USB_sys_state_t USBSystemState;
fnir_mode_state_t fnirModeState;
static FILE USBSerialStream;
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface = {
    .Config = {
        .ControlInterfaceNumber = INTERFACE_ID_CDC_CCI,
        .DataINEndpoint = {
            .Address = CDC_TX_EPADDR,
            .Size = CDC_TXRX_EPSIZE,
            .Banks = 1,
        },
        .DataOUTEndpoint = {
            .Address = CDC_RX_EPADDR,
            .Size = CDC_TXRX_EPSIZE,
            .Banks = 1,
        },
        .NotificationEndpoint = {
            .Address = CDC_NOTIFICATION_EPADDR,
            .Size = CDC_NOTIFICATION_EPSIZE,
            .Banks = 1,
        },
    },
};

/** Main function
*
* Program begins here upon bootup
*
* @return This function should never exit.
*/
int main(void) {
    uint8_t receivedByte;

    USBSystemState = USB_IDLE;
    fnirModeState = FNIR_STOP;

    spiInit();
    mainLedInit();

    sei();

    for (;;) {
        switch (USBSystemState) {
        case (USB_IDLE) :
            LED_TOGGLE();
            USB_Init();
            CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);
            break;
        case (USB_CONNECTED) :
            LED_ON();
            receivedByte = fgetc(&USBSerialStream);
            mainParseCommand(receivedByte);
            mainFnirScan();
            CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
            USB_USBTask();
            break;
        default :
            break;
        }
    }
}

/** Initializes LED output for debug and NIR leds.
*
*/
void mainLedInit(void) {
    LED_OFF();
    DDRB |= (1<<PB7);

    mainNirLedControl(FNIR_NULL, 0);
    DDRD |= 0xFF; // Turn all port d IO to outputs
}

/** Parses commands received over usb-serial from host computer
*
* Checks the received byte from the usb serial stream device
* \ref USBSerialStream and passes it through a switch statement.
* Able to start and stop continuous measurements with chars \c s & \c p.
*
* @param receivedByte ASCII char received via usb-serial to be parsed
* @return This function does not return a value.
*/
void mainParseCommand(uint8_t receivedByte) {
    // Handle messages from host
    switch (receivedByte) {
    case (EOF) :
        return;
        break;
    case ('s') :
        fprintf(&USBSerialStream, "Starting\r\n");
        fnirModeState = FNIR_IDLE;
    case ('p') :
        fprintf(&USBSerialStream, "Stopping\r\n");
        fnirModeState = FNIR_STOP;
    default :
        break;
    }
}

/** Handles measurement of subject
*
* Cycles through all 16 channels, taking measurements with both types of LEDs
* as well as a non-LED measurement to calculate an offset value.
*
* @return This function does not return a value.
*/
void mainFnirScan(void) {
    static uint8_t measurementChannelSelected = 0;
    static adcReturn_t voltageLevel[3];

    switch (fnirModeState) {
    case (FNIR_NULL) :
        fnirModeState = FNIR_730NM;
        break;
    case (FNIR_730NM) :
        mainNirLedControl(fnirModeState, measurementChannelSelected);
        voltageLevel[0] = mainTakeMeasurement(measurementChannelSelected);
        fnirModeState = FNIR_850NM;
        break;
    case (FNIR_850NM) :
        mainNirLedControl(fnirModeState, measurementChannelSelected);
        voltageLevel[1] = mainTakeMeasurement(measurementChannelSelected);
        fnirModeState = FNIR_IDLE;
        break;
    case (FNIR_IDLE) :
        mainNirLedControl(fnirModeState, measurementChannelSelected);
        voltageLevel[2] = mainTakeMeasurement(measurementChannelSelected);
        fnirModeState = FNIR_NULL;

        // Send measurement via USB.
        mainReportResult(measurementChannelSelected, voltageLevel);

        // Move on to next measurement channel, or start back at 0
        if (measurementChannelSelected < 16) {
            measurementChannelSelected++;
        } else {
            measurementChannelSelected = 0;
        }
        break;
    case (FNIR_STOP) :
        break;
    default:
        break;
    }
}

/** Controls selection of near infrared leds.
*
* Channel may be 0-3, all leds will turn off when
* fnirMode == FNIR_IDLE | FNIR_NULL | FNIR_STOP.
*
* @param fnirMode LED type to activate
* @param channel led to activate
*/
void mainNirLedControl(fnir_mode_state_t fnirMode, uint8_t channel) {
    uint8_t ledChannel;

    ledChannel = channel/4; // Determine LED pair from desired channel

    // Activate proper led for desired mode and channel
    if (fnirMode == FNIR_730NM) {
        switch (ledChannel) {
        case 0 :
            PORTD = (1<<PD0);
            break;
        case 1 :
            PORTD = (1<<PD2);
            break;
        case 2 :
            PORTD = (1<<PD4);
            break;
        case 3 :
            PORTD = (1<<PD6);
            break;
        }
    } else if (fnirMode == FNIR_850NM) {
        switch (ledChannel) {
        case 0 :
            PORTD = (1<<PD1);
            break;
        case 1 :
            PORTD = (1<<PD3);
            break;
        case 2 :
            PORTD = (1<<PD5);
            break;
        case 3 :
            PORTD = (1<<PD7);
            break;
        }
    } else {
        PORTD = 0x00; // Turn off all IO on port D
    }
}

/** Retrieves measurement from ADC
*
* Commands ADC to take measurement from selected channel with selected
* settings. Blocks until result is returned.
*
* @param channel channel of measurement to retrieve.
* @return measurement data
*/
adcReturn_t mainTakeMeasurement(uint8_t channel) {
    adcReturn_t adcReturnValue;
    adcChannelType_t adcChannel;

    // Determine multiplexer setup for desired channel
    switch (channel) {
    case 0 :
        adcChannel = UNIPOLAR_CH_0;
        break;
    case 1 :
        adcChannel = UNIPOLAR_CH_1;
        break;
    case 2 :
    case 4 :
        adcChannel = UNIPOLAR_CH_2;
        break;
    case 3 :
    case 5 :
        adcChannel = UNIPOLAR_CH_3;
        break;
    case 6 :
    case 8 :
        adcChannel = UNIPOLAR_CH_4;
        break;
    case 7 :
    case 9 :
        adcChannel = UNIPOLAR_CH_5;
        break;
    case 10 :
    case 12 :
        adcChannel = UNIPOLAR_CH_6;
        break;
    case 11 :
    case 13 :
        adcChannel = UNIPOLAR_CH_7;
        break;
    case 14 :
        adcChannel = UNIPOLAR_CH_9;
        break;
    case 15 :
        adcChannel = UNIPOLAR_CH_11;
        break;
    default :
        break;
    }

    CHIP_SELECT();

    // Command ADC to begin conversion
    (void) adcSelect(ENABLE,         // Enable adc
                     adcChannel,     // Select channel
                     REJECT_60HZ,    // Reject 60hz powerline noise
                     AUTO_CALIBRATE, // Slower but more accurate speed
                     GAIN_1X);       // Unity gain (no amplification)

    while (PORTB & (1<<PB3)) {} // Wait for conversion complete

    // Get return value while commanding ADC to shutdown
    adcReturnValue = adcSelect(DISABLE,
                               NULL_CH,
                               NULL_REJECTION,
                               NULL_SPEED,
                               NULL_GAIN);

    CHIP_DESELECT();

    return (adcReturnValue);
}

/** Reports CVS dataset of measurements over USB
*
* TODO: calculate estimated oxy content from received values
*
* @param channel channel measured
* @param adcReturnValue array of measurements taken from specific channel
*/
void mainReportResult(uint8_t channel, adcReturn_t *adcReturnValue) {
    uint16_t estimatedOxyContent;

    fprintf(&USBSerialStream, "%d,%d,%d,%d,%d\r\n", // CSV string
            ((uint16_t) channel),                   // Reported channel
            adcReturnValue[0].returnValue,          // 730nm result
            adcReturnValue[1].returnValue,          // 850nm result
            adcReturnValue[2].returnValue,          // Dark result
            estimatedOxyContent);                   // Calculated oxy value
}

/** Event handler for the library USB Connection event. 
*
* Stops connection attempts from being made after the host device enumerates
* the system and prints "Device connected" onto the serial terminal.
* 
* @return This function does not return a value.
*/
void EVENT_USB_Device_Connect(void) {
    fprintf(&USBSerialStream, "Device Connected\r\n");
    USBSystemState = USB_CONNECTED;
}

/** Event handler for the library USB Disconnection event.
*
* Starts looping device connection attempts when if device becomes
* disconnected.
*
* @return This function does not return a value.
*/
void EVENT_USB_Device_Disconnect(void) {
    USBSystemState = USB_IDLE;
}

/** Event handler for the library USB Configuration Changed event.
*
*
* @return This function does not return a value.
*/
void EVENT_USB_Device_ConfigurationChanged(void) {
    CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);
}

/** Event handler for the library USB Control Request reception event.
*
* @return This function does not return a value.
*/
void EVENT_USB_Device_ControlRequest(void) {
    CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}


