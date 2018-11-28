#ifndef INJECTOR_H
#define	INJECTOR_H

#define _XTAL_FREQ 1000000

#define WHITE_LED_ON() (LATC2 = 0) 
#define WHITE_LED_OFF() (LATC2 = 1)
#define RED_LED_ON() (LATC3 = 0)
#define RED_LED_OFF() (LATC3 = 1)
#define BLUE_LED_ON() (LATC4 = 0)
#define BLUE_LED_OFF() (LATC4 = 1)

void led_init();

void injector_init();
void injector_open();
void injector_close();

#endif	/* INJECTOR_H */

