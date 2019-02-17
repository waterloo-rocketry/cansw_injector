#include <stdio.h>
#include <stdlib.h>

#include "injector.h"

#include "canlib/can.h"
#include "canlib/can_common.h"
#include "canlib/pic18f26k83/pic18f26k83_can.h"
#include "canlib/message_types.h"

#include "mcc_generated_files/fvr.h"
#include "mcc_generated_files/adcc.h"
#include "mcc_generated_files/mcc.h"
#include <xc.h> // should be after any pragma statements

static void can_msg_handler(can_msg_t *msg);

int main(int argc, char** argv) {
    // MCC Generated Initializations
    SYSTEM_Initialize();
    OSCILLATOR_Initialize();
    FVR_Initialize();

    ADCC_Initialize();
    ADCC_DisableContinuousConversion();

    // Enable global interrupts
    INTCON0bits.GIE = 1;

    // local initializers
    led_init();
    injector_init();

    // set up CAN module
    can_timing_t can_setup;
    can_setup.brp = 0;
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
        status_ok &= check_valve_status(); // this might need rework
 
        if (status_ok) {
            can_msg_t board_stat_msg;
            board_stat_msg.sid = MSG_GENERAL_BOARD_STATUS | BOARD_UNIQUE_ID;
            board_stat_msg.data_len = 0;
            can_send(&board_stat_msg, 0);   // send at low priority
        }

        // TODO: Deal with incoming CAN messages synchronously
    }
    return (EXIT_SUCCESS);
}

static void interrupt interrupt_handler() {
    if (PIR5) {
        // we've received a CAN-related interrupt
        can_handle_interrupt();
    }
}

// This is called from within can_handle_interrupt()
static void can_msg_handler(can_msg_t *msg) {
    WHITE_LED_OFF();
    RED_LED_OFF();
    BLUE_LED_OFF();

    // vent control logic will go in here
    if (msg->sid == 0xa) {
        WHITE_LED_ON();
    } else if (msg->sid == 0xb) {
        RED_LED_ON();
    } else {
        RED_LED_ON();
    }
}
