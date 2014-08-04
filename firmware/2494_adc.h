/******************************************************************************
* 2494 ADC Communication Library                                              *
* By Jeremy Ruhland                                                           *
*                                                                             *
* The adc will begin its measurement after the program calls adcSelect().     *
* The proper enumerated type names are defined below. When appropriate, such  *
* as when shutting down the adc or requesting a repeat measurement, null      *
* types may be used, which will have no effect on the data sent to the adc.   *
*                                                                             *
* adcSelect() returns a struct which contains a conversion status, polarity   *
* voltage level, which can be used together to determine error conditions and *
* adc state.                                                                  *
*                                                                             *
* The following is an example of capturing a positive unipolar voltage level  *
* referenced to common (ground) and passing it to doSomethingWithVoltage() :  *
*                                                                             *
* adcReturn_t voltageLevel; // adcSelect returns a struct containing voltage, *
*                           // conversion status and polarity information     *
*                                                                             *
* voltageLevel = adcSelect(ENABLE,         // Enable adc                      *
*                          UNIPOLAR_CH_0,  // Select channel 0, ref to common *
*                          REJECT_60HZ,    // Reject 60hz powerline noise     *
*                          AUTO_CALIBRATE, // Slower but more accurate speed  *
*                          GAIN_1X);       // Unity gain (no amplification)   *
*                                                                             *
* doSomethingWithVoltage(voltageLevel.returnValue); // Pass voltage to our    *
*                                                   // function               *
*                                                                             *
* And this is an example of re-running the last conversion:                   *
*                                                                             *
* voltageLevel = adcSelect(REPEAT,         // Repeat last conversion          *
*                          NULL_CH,        // Don't change channel            *
*                          NULL_REJECTION, // or rejection                    *
*                          NULL_SPEED,     // or speed                        *
*                          NULL_GAIN);     // or gain                         *
*                                                                             *
* doSomethingWithVoltage(voltageLevel.returnValue);                           *
*                                                                             *
******************************************************************************/

typedef struct {
    enum {
        ONGOING,
        FINISHED
    } conversionStatus;
    enum {
        POSITIVE,
        NEGATIVE
    } returnSign;
    uint16_t returnValue;
} adcReturn_t;

typedef enum adcState {
    DISABLE,
    ENABLE,
    REPEAT
} adcState_t;

typedef enum adcChannelType {
    BIPOLAR_CH_0_1,
    BIPOLAR_CH_1_0,
    BIPOLAR_CH_2_3,
    BIPOLAR_CH_3_2,
    BIPOLAR_CH_4_5,
    BIPOLAR_CH_5_4,
    BIPOLAR_CH_6_7,
    BIPOLAR_CH_7_6,
    BIPOLAR_CH_8_9,
    BIPOLAR_CH_9_8,
    BIPOLAR_CH_10_11,
    BIPOLAR_CH_11_10,
    BIPOLAR_CH_12_13,
    BIPOLAR_CH_13_12,
    BIPOLAR_CH_14_15,
    BIPOLAR_CH_15_14,
    UNIPOLAR_CH_0,
    UNIPOLAR_CH_1,
    UNIPOLAR_CH_2,
    UNIPOLAR_CH_3,
    UNIPOLAR_CH_4,
    UNIPOLAR_CH_5,
    UNIPOLAR_CH_6,
    UNIPOLAR_CH_7,
    UNIPOLAR_CH_8,
    UNIPOLAR_CH_9,
    UNIPOLAR_CH_10,
    UNIPOLAR_CH_11,
    UNIPOLAR_CH_12,
    UNIPOLAR_CH_13,
    UNIPOLAR_CH_14,
    UNIPOLAR_CH_15,
    INTERNAL_TEMP_CH,
    NULL_CH
} adcChannelType_t;

typedef enum adcRejectionMode {
    REJECT_50HZ,
    REJECT_60HZ,
    REJECT_50HZ_60HZ,
    NULL_REJECTION
} adcRejectionMode_t;

typedef enum adcSpeed {
    AUTO_CALIBRATE,
    DOUBLE_SPEED,
    NULL_SPEED
} adcSpeed_t;

typedef enum adcGain {
    GAIN_1X,
    GAIN_2X,
    GAIN_4X,
    GAIN_8X,
    GAIN_16X,
    GAIN_32X,
    GAIN_64X,
    GAIN_128X,
    NULL_GAIN
} adcGain_t;

extern void adcInit(void);

extern adcReturn_t adcSelect(adcState_t adcState,
                             adcChannelType_t adcChannelType,
                             adcRejectionMode_t adcRejectionMode,
                             adcSpeed_t adcSpeed,
                             adcGain_t adcGain);
