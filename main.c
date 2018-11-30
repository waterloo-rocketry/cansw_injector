#include <stdio.h>
#include <stdlib.h>

#include "injector.h"
#include "mcc_generated_files/fvr.h"
#include "mcc_generated_files/adcc.h"
#include "mcc_generated_files/mcc.h"
#include <xc.h> // should be after any pragma statements

int main(int argc, char** argv) {
    // MCC Generated Initializations
    SYSTEM_Initialize();
    OSCILLATOR_Initialize();
    FVR_Initialize();

    ADCC_Initialize();
    ADCC_DisableContinuousConversion();

    // local initializers
    led_init();
    injector_init();

    while (1) {
        adc_result_t batt_raw = ADCC_GetSingleConversion(channel_VBAT);
        float batt_voltage = batt_raw * 5.00 / 4096.0;

        adc_result_t sense_raw = ADCC_GetSingleConversion(channel_VSENSE);
        float curr_draw = (sense_raw * 5.00 / 4096.0) / (100 * 0.2);

        BLUE_LED_ON();
        __delay_ms(200);
        BLUE_LED_OFF();
        __delay_ms(200);
    }
    return (EXIT_SUCCESS);
}

