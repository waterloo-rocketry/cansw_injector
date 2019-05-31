#ifndef INJECTOR_H
#define	INJECTOR_H

#include <stdbool.h>
#include "canlib/message_types.h"

#define MAX_LOOP_TIME_DIFF_ms 250

// [<--------injector close------->] [<--------injector open-------->]
// HIGH_SIDE_A  LOW_SIDE_B LIM_CLOSE HIGH_SIDE_B  LOW_SIDE_A  LIM_OPEN
// limit switch signals are active low
#define EXPECTED_OPEN_PINREADS 0b001110
#define EXPECTED_CLOSED_PINREADS 0b110001

#define WHITE_LED_ON() (LATC2 = 0) 
#define WHITE_LED_OFF() (LATC2 = 1)
#define RED_LED_ON() (LATC3 = 0)
#define RED_LED_OFF() (LATC3 = 1)
#define BLUE_LED_ON() (LATC4 = 0)
#define BLUE_LED_OFF() (LATC4 = 1)

void led_init(void);

// Initialize pins for injector valve control
void injector_init(void);

// Close the injector valve. Asynchronous.
void injector_close(void);

// Open the injector valve. Asynchronous.
void injector_open(void);

// Kill all power to the injector pins
void injector_depower(void);

// Open the injector valve a little bit to act as an emergency vent. This
// function is SYNCHRONOUS and will block while the valve is opened.
// Upon completion, limit switch state will return VALVE_UNK.
void injector_jog(void);

// Report the current and requested valve state over CAN
void injector_send_status(enum VALVE_STATE req_state);

#endif	/* INJECTOR_H */
