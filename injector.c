#include "canlib/can.h"
#include "canlib/can_common.h"
#include "canlib/pic18f26k83/pic18f26k83_can.h"
#include "canlib/message_types.h"
#include "canlib/util/can_tx_buffer.h"

#include "mcc_generated_files/mcc.h"

#include "injector.h"
#include "timer.h"

#include <xc.h>

static enum VALVE_STATE current_valve_state = VALVE_UNK;

void led_init(void) {
    TRISC2 = 0;     // set as output
    LATC2 = 1;      // turn it off

    TRISC3 = 0;
    LATC3 = 1;

    TRISC4 = 0;
    LATC4 = 1;
}

void injector_init(void) {
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

void injector_close(void) {
    if (current_valve_state == VALVE_CLOSED) {
        return;
    } else {
        WHITE_LED_OFF();
        LATB4 = 0;
        LATB5 = 0;
        LATB2 = 0;
        LATB3 = 0;

        __delay_us(200);
        LATB4 = 1;  // high side A -> high
        LATB3 = 1;  // low side B -> high
        current_valve_state = VALVE_CLOSED;
    }
}

void injector_open(void) {
    if (current_valve_state == VALVE_OPEN) {
        return;
    } else {
        WHITE_LED_ON();
        LATB4 = 0;
        LATB5 = 0;
        LATB2 = 0;
        LATB3 = 0;

        __delay_us(200);
        LATB5 = 1;  // high side B -> high
        LATB2 = 1;  // low side A -> high
        current_valve_state = VALVE_OPEN;
    }
}

void injector_depower(void) {
    WHITE_LED_OFF();
    //turn everything off
    LATB4 = 0;
    LATB5 = 0;
    LATB2 = 0;
    LATB3 = 0;

    current_valve_state = VALVE_UNK;
}

void injector_jog(void) {
    // don't jog the valve twice in a row, since then it'd be fully open
    if (current_valve_state == VALVE_ILLEGAL) {
        return;
    } else {
        // turn everything off for just a moment, avoid shoot-through
        LATB4 = 0;
        LATB5 = 0;
        LATB2 = 0;
        LATB3 = 0;
        __delay_us(200);

        // open the valve for 200ms, then turn everything off. Empirically this
        // seems to open the valve approximately 45 degrees, which is open enough
        // to allow air to pass but not open enough to be able to see through
        injector_open();
        __delay_ms(200);
        LATB4 = 0;
        LATB5 = 0;
        LATB2 = 0;
        LATB3 = 0;

        current_valve_state = VALVE_ILLEGAL;
    }
}

void injector_send_status(enum VALVE_STATE req_state) {
    enum VALVE_STATE curr_state;
    
    // RB0 = LIM_CLOSE, RB1 = LIM_OPEN - refer to schematic
    // Limit switch signals are active low
    uint8_t limit_pins = PORTBbits.RB0 << 1 | PORTBbits.RB1;
    switch (limit_pins) {
        // closed switch pressed only
        case 0b01:
            curr_state = VALVE_CLOSED;
            break;
         
        // open switch pressed only
        case 0b10:
            curr_state = VALVE_OPEN;
            break;
        
        // neither switch pressed
        case 0b11:
            curr_state = VALVE_UNK;
            break;
            
        // 0b11 - both switches pressed
        default:
            curr_state = VALVE_ILLEGAL;
            break;
    }
    
    can_msg_t stat_msg;
    build_valve_stat_msg(millis(), curr_state, req_state, MSG_INJ_VALVE_STATUS, &stat_msg);
    txb_enqueue(&stat_msg);
}
