#include <xc.h>
#include "injector.h"

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
