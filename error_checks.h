#ifndef ERROR_CHECKS_H
#define	ERROR_CHECKS_H

#include <stdbool.h>
#include "canlib/message_types.h"

#define INJ_BATT_UNDERVOLTAGE_THRESHOLD_mV 10000
#define INJ_BATT_OVERVOLTAGE_THRESHOLD_mV 14000
#define INJ_OVERCURRENT_THRESHOLD_mA 100

// Board status checks
// NOTE: These functions will asynchronously send CAN messages upon error

/*
 * Checks voltage coming off the injector valve battery. If the voltage is below
 * our acceptable threshold, a CAN status message is sent.
 * 
 * Returns true if battery voltage is OK, false otherwise
 */
bool check_battery_voltage_error(void);

/*
 * Checks the current draw of the board from the bus 5V line. This is only supposed
 * to power the logic part of the board and does not power the valve. If the current
 * is above our acceptable threshold, a CAN status message is sent.
 */
bool check_bus_current_error(void);

bool check_valve_pin_error(enum VALVE_STATE req_state);

#endif	/* ERROR_CHECKS_H */

