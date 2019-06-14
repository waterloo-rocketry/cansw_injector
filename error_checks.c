#include "canlib/can.h"
#include "canlib/can_common.h"
#include "canlib/pic18f26k83/pic18f26k83_can.h"
#include "canlib/message_types.h"
#include "canlib/util/can_tx_buffer.h"

#include "mcc_generated_files/fvr.h"
#include "mcc_generated_files/adcc.h"
#include "mcc_generated_files/mcc.h"

#include "timer.h"
#include "injector.h"
#include "error_checks.h"

#include <stdlib.h>

bool check_battery_voltage_error(void) {
    adc_result_t batt_raw = ADCC_GetSingleConversion(channel_VBAT);

    // Vref: 4.096V, Resolution: 12 bits -> raw ADC value is precisely in mV
    uint32_t batt_voltage_mV = (uint16_t)batt_raw;

    // get the un-scaled battery voltage (voltage divider)
    // we don't care too much about precision - some truncation is fine
    batt_voltage_mV = batt_voltage_mV * 45 / 10;

    if (batt_voltage_mV < INJ_BATT_UNDERVOLTAGE_THRESHOLD_mV
            || batt_voltage_mV > INJ_BATT_OVERVOLTAGE_THRESHOLD_mV) {

        uint32_t timestamp = millis();
        uint8_t batt_data[2] = {0};
        batt_data[0] = (batt_voltage_mV >> 8) & 0xff;
        batt_data[1] = (batt_voltage_mV >> 0) & 0xff;
        enum BOARD_STATUS error_code = batt_voltage_mV < INJ_BATT_UNDERVOLTAGE_THRESHOLD_mV
                ? E_BATT_UNDER_VOLTAGE
                : E_BATT_OVER_VOLTAGE;

        can_msg_t error_msg;
        build_board_stat_msg(timestamp, error_code, batt_data, 2, &error_msg);
        txb_enqueue(&error_msg);
        return false;
    }

    // also send the battery voltage as a sensor data message
    // this may or may not be the best place to put this
    can_msg_t batt_msg;
    build_analog_data_msg(millis(), SENSOR_INJ_BATT, (uint16_t) batt_voltage_mV, &batt_msg);
    txb_enqueue(&batt_msg);

    // things look ok
    return true;
}

bool check_bus_current_error(void) {
    adc_result_t sense_raw_mV = ADCC_GetSingleConversion(channel_VSENSE);
    uint16_t curr_draw_mA = (sense_raw_mV) / 20;

    if (curr_draw_mA > INJ_OVERCURRENT_THRESHOLD_mA) {
        uint32_t timestamp = millis();
        uint8_t curr_data[2] = {0};
        curr_data[0] = (curr_draw_mA >> 8) & 0xff;
        curr_data[1] = (curr_draw_mA >> 0) & 0xff;

        can_msg_t error_msg;
        build_board_stat_msg(timestamp, E_BUS_OVER_CURRENT, curr_data, 2, &error_msg);
        txb_enqueue(&error_msg);
        return false;
    }

    // things look ok
    return true;
}

// [<--------injector close------->] [<--------injector open-------->]
// HIGH_SIDE_A  LOW_SIDE_B LIM_CLOSE HIGH_SIDE_B  LOW_SIDE_A  LIM_OPEN
bool check_valve_pin_error(enum VALVE_STATE req_state) {
    uint8_t valve_pin_state =
        PORTBbits.RB4 << 5 |   // HIGH SIDE A
        PORTBbits.RB3 << 4 |   // LOW SIDE B
        PORTBbits.RB0 << 3 |   // LIM CLOSE
        PORTBbits.RB5 << 2 |   // HIGH SIDE B
        PORTBbits.RB2 << 1 |   // LOW SIDE A
        PORTBbits.RB1 << 0 ;   // LIM OPEN

    // not the most compact way to right this but whatevs
    if (req_state == VALVE_OPEN) {
        if (valve_pin_state != EXPECTED_OPEN_PINREADS) {
            uint8_t error_data[2] = {0};
            error_data[0] = EXPECTED_OPEN_PINREADS;
            error_data[1] = valve_pin_state;

            can_msg_t error_msg;
            build_board_stat_msg(millis(), E_VALVE_STATE, error_data, 2, &error_msg);
            txb_enqueue(&error_msg);

            return false;
       }
    } else if (req_state == VALVE_CLOSED) {
        if (valve_pin_state != EXPECTED_CLOSED_PINREADS) {
            uint8_t error_data[2] = {0};
            error_data[0] = EXPECTED_CLOSED_PINREADS;
            error_data[1] = valve_pin_state;

            can_msg_t error_msg;
            build_board_stat_msg(millis(), E_VALVE_STATE, error_data, 2, &error_msg);
            txb_enqueue(&error_msg);

            return false;
       }
    }

    return true;
}
