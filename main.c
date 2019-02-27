#include <stdio.h>
#include <stdlib.h>

#include "injector.h"
#include "timer.h"

#include "canlib/can.h"
#include "canlib/can_common.h"
#include "canlib/pic18f26k83/pic18f26k83_can.h"
#include "canlib/message_types.h"

#include "mcc_generated_files/fvr.h"
#include "mcc_generated_files/adcc.h"
#include "mcc_generated_files/mcc.h"
#include <xc.h> // should be after any pragma statements

static void can_msg_handler(can_msg_t *msg);
static void send_status_ok(void);

// Follows VALVE_STATE in message_types.h
// SHOULD ONLY BE MODIFIED IN ISR
static uint8_t requested_valve_state = VALVE_OPEN;

int main(int argc, char** argv) {
    // MCC Generated Initializations
    SYSTEM_Initialize();
    OSCILLATOR_Initialize();
    FVR_Initialize();

    ADCC_Initialize();
    ADCC_DisableContinuousConversion();

    // Enable global interrupts
    INTCON0bits.GIE = 1;

    // Set up CAN TX
    TRISC0 = 0;
    RC0PPS = 0x33;

    // Set up CAN RX
    TRISC1 = 1;
    ANSELC1 = 0;
    CANRXPPS = 0x11;

    // local initializers
    led_init();
    timer0_init();
    injector_init();

    // set up CAN module
    can_timing_t can_setup;
    can_setup.brp = 11;
    can_setup.sjw = 3;
    can_setup.btlmode = 0x01;
    can_setup.sam = 0;
    can_setup.seg1ph = 0x04;
    can_setup.prseg = 0;
    can_setup.seg2ph = 0x04;
    can_init(&can_setup, can_msg_handler);

    // main event loop
    while (1) {
        BLUE_LED_ON();
        __delay_ms(100);
        BLUE_LED_OFF();
        __delay_ms(100);

        bool status_ok = true;
        status_ok &= check_battery_voltage();
        status_ok &= check_current_draw();
        status_ok &= check_valve_status();

        if (status_ok) {
            send_status_ok();
        }

        // "thread safe" because main loop should never write to requested_valve_state
        switch (requested_valve_state) {
            //TODO, if the valve is already open, ideally we wouldn't call injector_open again
            case VALVE_OPEN:
                injector_open();
                break;
            case VALVE_CLOSED:
                injector_close();
                break;

            // should never get here
            default:
                // spit a message
                break;
        }

        // TODO: report valve state

    }

    return (EXIT_SUCCESS);
}

static void interrupt interrupt_handler() {
    // we've received a CAN related interrupt
    if (PIR5) {
        can_handle_interrupt();
    }

    // Timer0 has overflowed - update millis() function
    // This happens approximately every 500us
    if (PIE3bits.TMR0IE == 1 && PIR3bits.TMR0IF == 1) {
        timer0_handle_interrupt();
        PIR3bits.TMR0IF = 0;
    }
}

// This is called from within can_handle_interrupt()
static void can_msg_handler(can_msg_t *msg) {
    uint16_t msg_type = get_message_type(msg);
    switch (msg_type) {
        case MSG_GENERAL_CMD:
            // nothing right now
            break;

        case MSG_INJ_VALVE_CMD:
            // see message_types.h for message format
            requested_valve_state = msg->data[3];
            break;

        case MSG_LEDS_ON:
            RED_LED_ON();
            BLUE_LED_ON();
            WHITE_LED_ON();
            break;

        case MSG_LEDS_OFF:
            RED_LED_OFF();
            BLUE_LED_OFF();
            WHITE_LED_OFF();
            break;

        // all the other ones - do nothing
        case MSG_VENT_VALVE_CMD:
        case MSG_DEBUG_MSG:
        case MSG_DEBUG_PRINTF:
        case MSG_VENT_VALVE_STATUS:
        case MSG_INJ_VALVE_STATUS:
        case MSG_SENSOR_ACC:
        case MSG_SENSOR_GYRO:
        case MSG_SENSOR_MAG:
        case MSG_SENSOR_ANALOG:
        case MSG_GENERAL_BOARD_STATUS:
            break;

        // illegal message type - should never get here
        default:
            // send a message or something
            break;
    }
}

// Send a CAN message with nominal status
static void send_status_ok(void) {
    can_msg_t board_stat_msg;
    board_stat_msg.sid = MSG_GENERAL_BOARD_STATUS | BOARD_UNIQUE_ID;

    // capture the most recent timestamp
    uint32_t last_millis = millis();

    // paste in the timestamp one byte at a time. Most significant byte goes in data[0].
    board_stat_msg.data[0] = (last_millis >> 16) & 0xff;
    board_stat_msg.data[1] = (last_millis >> 8) & 0xff;
    board_stat_msg.data[2] = (last_millis >> 0) & 0xff;

    // set the error code
    board_stat_msg.data[3] = E_NOMINAL;

    // 3 byte timestamp + 1 byte error code
    board_stat_msg.data_len = 4;

    // send it off at low priority
    can_send(&board_stat_msg, 0);
}
