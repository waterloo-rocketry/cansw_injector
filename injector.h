#ifndef INJECTOR_H
#define	INJECTOR_H

#include <stdbool.h>

#define _XTAL_FREQ 1000000

#define INJ_BATT_UNDERVOLTAGE_THRESHOLD_mV 10000
#define INJ_BATT_OVERVOLTAGE_THRESHOLD_mV 14000
#define INJ_OVERCURRENT_THRESHOLD_mA 100

#define WHITE_LED_ON() (LATC2 = 0) 
#define WHITE_LED_OFF() (LATC2 = 1)
#define RED_LED_ON() (LATC3 = 0)
#define RED_LED_OFF() (LATC3 = 1)
#define BLUE_LED_ON() (LATC4 = 0)
#define BLUE_LED_OFF() (LATC4 = 1)

void led_init();

// Injector valve control
void injector_init();
void injector_open();
void injector_close();

// Board status checks
// NOTE: These functions will asynchronously send CAN messages upon error

/*
 * Checks voltage coming off the injector valve battery. If the voltage is below
 * our acceptable threshold, a CAN status message is sent.
 * 
 * Returns true if battery voltage is OK, false otherwise
 */
bool check_battery_voltage();

/*
 * Checks the current draw of the board from the bus 5V line. This is only supposed
 * to power the logic part of the board and does not power the valve. If the current
 * is above our acceptable threshold, a CAN status message is sent.
 */
bool check_current_draw();

bool check_valve_status();

#endif	/* INJECTOR_H */

