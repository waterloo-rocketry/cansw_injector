#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "injector.h"

int main(int argc, char** argv) {
    led_init();
    injector_init();
    
    while (1) {
        BLUE_LED_ON();
        __delay_ms(200);
        BLUE_LED_OFF();
        __delay_ms(200);
    }
    return (EXIT_SUCCESS);
}

