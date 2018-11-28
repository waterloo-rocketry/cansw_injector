#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "injector.h"

int main(int argc, char** argv) {
    led_init();
    injector_init();
    
    while (1);
    return (EXIT_SUCCESS);
}

