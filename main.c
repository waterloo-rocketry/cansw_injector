#include <stdio.h>
#include <stdlib.h>
#include "config.h"

#define _XTAL_FREQ 1000000

#define WHITE_LED_ON (LATC2 = 0) 
#define WHITE_LED_OFF (LATC2 = 1)
#define RED_LED_ON (LATC3 = 0)
#define RED_LED_OFF (LATC3 = 1)
#define BLUE_LED_ON (LATC4 = 0)
#define BLUE_LED_OFF (LATC4 = 1)

static void led_init(){
    TRISC2 = 0;     // set as output
    LATC2 = 1;      // turn it off
    
    TRISC3 = 0;
    LATC3 = 1;
    
    TRISC4 = 0;
    LATC4 = 1;
}

int main(int argc, char** argv) {
    led_init();
    
    while (1) {
        WHITE_LED_ON;
        RED_LED_ON;
        BLUE_LED_ON;
        __delay_ms(1000);
        
        WHITE_LED_OFF;
        RED_LED_OFF;
        BLUE_LED_OFF;
        __delay_ms(1000);
    }
    
    return (EXIT_SUCCESS);
}

