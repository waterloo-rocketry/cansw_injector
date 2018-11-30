/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65.2
        Device            :  PIC18F26K83
        Driver Version    :  2.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.45
        MPLAB 	          :  MPLAB X 4.15	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set channel_VBAT aliases
#define channel_VBAT_TRIS                 TRISAbits.TRISA0
#define channel_VBAT_LAT                  LATAbits.LATA0
#define channel_VBAT_PORT                 PORTAbits.RA0
#define channel_VBAT_WPU                  WPUAbits.WPUA0
#define channel_VBAT_OD                   ODCONAbits.ODCA0
#define channel_VBAT_ANS                  ANSELAbits.ANSELA0
#define channel_VBAT_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define channel_VBAT_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define channel_VBAT_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define channel_VBAT_GetValue()           PORTAbits.RA0
#define channel_VBAT_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define channel_VBAT_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define channel_VBAT_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define channel_VBAT_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define channel_VBAT_SetPushPull()        do { ODCONAbits.ODCA0 = 0; } while(0)
#define channel_VBAT_SetOpenDrain()       do { ODCONAbits.ODCA0 = 1; } while(0)
#define channel_VBAT_SetAnalogMode()      do { ANSELAbits.ANSELA0 = 1; } while(0)
#define channel_VBAT_SetDigitalMode()     do { ANSELAbits.ANSELA0 = 0; } while(0)

// get/set channel_VSENSE aliases
#define channel_VSENSE_TRIS                 TRISAbits.TRISA1
#define channel_VSENSE_LAT                  LATAbits.LATA1
#define channel_VSENSE_PORT                 PORTAbits.RA1
#define channel_VSENSE_WPU                  WPUAbits.WPUA1
#define channel_VSENSE_OD                   ODCONAbits.ODCA1
#define channel_VSENSE_ANS                  ANSELAbits.ANSELA1
#define channel_VSENSE_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define channel_VSENSE_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define channel_VSENSE_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define channel_VSENSE_GetValue()           PORTAbits.RA1
#define channel_VSENSE_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define channel_VSENSE_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define channel_VSENSE_SetPullup()          do { WPUAbits.WPUA1 = 1; } while(0)
#define channel_VSENSE_ResetPullup()        do { WPUAbits.WPUA1 = 0; } while(0)
#define channel_VSENSE_SetPushPull()        do { ODCONAbits.ODCA1 = 0; } while(0)
#define channel_VSENSE_SetOpenDrain()       do { ODCONAbits.ODCA1 = 1; } while(0)
#define channel_VSENSE_SetAnalogMode()      do { ANSELAbits.ANSELA1 = 1; } while(0)
#define channel_VSENSE_SetDigitalMode()     do { ANSELAbits.ANSELA1 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/