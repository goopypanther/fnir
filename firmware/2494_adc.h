/******************************************************************************
* 2494 ADC Communication Library                                              *
* By Jeremy Ruhland                                                           *
*                                                                             *
******************************************************************************/

#include "includes.h"

extern void adcInit(void);
extern adcReturn_t adcSelect(adcState_t adcState,
                             adcChannelType_t adcChannelType,
                             adcRejectionMode_t adcRejectionMode,
                             adcSpeed_t adcSpeed,
                             adcGain_t adcGain);

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
    MONOPOLAR_CH_0,
    MONOPOLAR_CH_1,
    MONOPOLAR_CH_2,
    MONOPOLAR_CH_3,
    MONOPOLAR_CH_4,
    MONOPOLAR_CH_5,
    MONOPOLAR_CH_6,
    MONOPOLAR_CH_7,
    MONOPOLAR_CH_8,
    MONOPOLAR_CH_9,
    MONOPOLAR_CH_10,
    MONOPOLAR_CH_11,
    MONOPOLAR_CH_12,
    MONOPOLAR_CH_13,
    MONOPOLAR_CH_14,
    MONOPOLAR_CH_15,
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
} adcSpeed_t

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
} adcGain_t
