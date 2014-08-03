/******************************************************************************
* fNIR Brain Imager                                                           *
* Jeremy Ruhland                                                              *
*                                                                             *
* Board control software                                                      *
******************************************************************************/

#include "includes.h"

typedef enum {USB_CONNECTED, USB_IDLE} USB_sys_state_t;

void mainParseCommand(void);
void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);

static FILE USBSerialStream;
USB_sys_state_t USBSystemState;

USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface = {
    .Config = {
        .ControlInterfaceNumber = 0,
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

int main(void) {
    USBSystemState = USB_IDLE;

    spiInit();
    mainLedInit();

    sei();

    for (;;) {
        switch (USBSystemState) {
        case (USB_IDLE) :
            USB_Init();
            CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);
            break;
        case (USB_CONNECTED) :
            mainParseCommand();
            CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
            USB_USBTask();
            break;
        }
    }
}

void mainParseCommand(void) {
    // Handle messages from host
    uint8_t receivedByte;

    receivedByte = fgetc(&USBSerialStream);

    if((receivedByte == EOF) {
        return;
    }

    if (ReceivedByte == 'b') {
        if(buzzer_enabled) {
            fprintf(&USBSerialStream, "Buzzer on\r\n");
        } else {
            fprintf(&USBSerialStream, "Buzzer off\r\n");
        }
    }
}

/* Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void) {
    USBSystemState = USB_CONNECTED;

    fprintf(&USBSerialStream, "Device Connected\r\n");
}

/* Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void) {
    USBSystemState = USB_IDLE;
}

/* Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void) {
    CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);
}

/* Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void) {
    CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}


