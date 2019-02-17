#include <xc.h>

#include "injector.h"

#include "canlib/can.h"
#include "canlib/can_common.h"
#include "canlib/pic18f26k83/pic18f26k83_can.h"
#include "canlib/message_types.h"

#include "mcc_generated_files/fvr.h"
#include "mcc_generated_files/adcc.h"
#include "mcc_generated_files/mcc.h"

void led_init(){
    TRISC2 = 0;     // set as output
    LATC2 = 1;      // turn it off
    
    TRISC3 = 0;
    LATC3 = 1;
    
    TRISC4 = 0;
    LATC4 = 1;
}

void injector_init() {
    // Set the injector control pins to outputs and drive them low  
    // HIGH_SIDE_A
    TRISB4 = 0;
    LATB4 = 0;
    
    // HIGH_SIDE_B
    TRISB5 = 0;
    LATB5 = 0;
    
    // LOW_SIDE_A
    TRISB2 = 0;
    LATB2 = 0;
    
    // LOW_SIDE_B
    TRISB3 = 0;
    LATB3 = 0;
    
    // Set the limit switch pins as digital inputs
    // LIM_OPEN
    TRISB1 = 1;
    ANSELB1 = 0;
    
    // LIM_CLOSED
    TRISB0 = 1;
    ANSELB0 = 0;
}

void injector_close() {
    // turn everything off for a bit to avoid shoot-through
    LATB4 = 0;
    LATB5 = 0;
    LATB2 = 0;
    LATB3 = 0;
    
    __delay_us(200);
    LATB4 = 1;  // high side A -> high
    LATB3 = 1;  // low side B -> high
}

void injector_open() {
    // turn everything off for a bit to avoid shoot-through
    LATB4 = 0;
    LATB5 = 0;
    LATB2 = 0;
    LATB3 = 0;
    
    __delay_us(200);
    LATB5 = 1;  // high side B -> high
    LATB2 = 1;  // low side A -> high
}

bool check_battery_voltage() {
    adc_result_t batt_raw = ADCC_GetSingleConversion(channel_VBAT);
    float batt_voltage = batt_raw * 5.00 / 4096.0;  // get the voltage seen by the MCU
    batt_voltage *= (10 + 27) / 10;                 // get undivided battery voltage
    
    if (batt_voltage < INJ_BATT_UNDERVOLTAGE_THRESHOLD_V) {
        can_msg_t error_msg;
        error_msg.sid = MSG_GENERAL_BOARD_STATUS | BOARD_UNIQUE_ID;
        error_msg.data_len = 0;     // FIXME when message format is nailed down
        can_send(&error_msg, 3);    // send at high priority
        return false;
    }
    
    // things look ok
    return true;
}

bool check_current_draw() {
    adc_result_t sense_raw = ADCC_GetSingleConversion(channel_VSENSE);
    float curr_draw = (sense_raw * 5.00 / 4096.0) / (100 * 0.2);
    curr_draw *= 1000 / (100 * 0.2);    // get current draw in mA
    
    if (curr_draw > INJ_OVERCURRENT_THRESHOLD_MA) {
        can_msg_t error_msg;
        error_msg.sid = MSG_GENERAL_BOARD_STATUS | BOARD_UNIQUE_ID;
        error_msg.data_len = 0;     // FIXME when message format is nailed down
        can_send(&error_msg, 3);    // send at high priority
        return false;
    }
    
    // things look ok
    return true;
}

bool check_valve_status() {
    // nothing rn
    return true;
}
