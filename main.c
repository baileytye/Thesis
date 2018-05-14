/*
 * File:   main.c
 * Author: Bailey Tye
 *
 * Created on March 28, 2018, 11:53 AM
 * Last Edited: April 24, 2018
 */

/* ------------------------ Built-in Library Headers ------------------------ */
#include "xc.h"
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <libpic30.h>

/* ---------------------- User-Defined Library Headers ---------------------- */
#include "Configuration.h"
#include "macros.h"



/*-----------------------User Definitions------------------------------*/


/* Pin Mapping
 * 
 * B15 - OSCILLATOR_OUTPUT - Output of internal oscillator, used to control the 
 *       data transmission.  
 * B14 - 
 * B13 - 
 * B8 - 
 * B7 - 
 * B1 -  
 * 
 */


#define INPUT 1
#define HIGH_IMPEDANCE 1
#define OUTPUT 0

#define CLOSED 1
#define OPEN 0

#define ON 1
#define OFF 0



#define OSCILLATOR_OUTPUT_DIR TRISBbits.TRISB15
#define OSCILLATOR_OUTPUT_EN REFOCONbits.ROEN

#define SWITCH_ONE_DIR TRISBbits.TRISB14
#define SWITCH_ONE_SET LATBbits.LATB14

#define SWITCH_TWO_DIR TRISBbits.TRISB13
#define SWITCH_TWO_SET LATBbits.LATB13

#define POWER_DETECTION_DIR TRISBbits.TRISB7
#define POWER_DETECTION_READ PORTBbits.RB7

#define TRANSMIT_EN_DIR TRISBbits.TRISB8
#define TRANSMIT_EN_SET LATBbits.LATB8 

#define TRANSMIT_CAP_DIR TRISBbits.TRISB1
#define TRANSMIT_CAP_SET LATBbits.LATB1




//-----------------TIMER DEFINITIONS---------------//

#define FCY 31400UL       // instruction cycle Hrtz

// libpic30 delays 
#define __delay_ms(d) \
  { __delay32( (unsigned long) (((unsigned long long) d)*(FCY)/1000ULL)); }
#define __delay_us(d) \
  { __delay32( (unsigned long) (((unsigned long long) d)*(FCY)/1000000ULL)); }


//--------------- USER MACROS------------------//
#define ONE(void) ({\
        REFOCONbits.ROEN = 1;\
        asm("nop");\
        asm("nop");\
        asm("nop");\
        REFOCONbits.ROEN = 0;\
           })




int main(void) {
    
    //Setup oscillator output for transmission
    OSCILLATOR_OUTPUT_DIR = OUTPUT;
    REFOCONbits.ROSSLP = 0;
    REFOCONbits.ROSEL = 0;
    REFOCONbits.RODIV = 0b0001;
    OSCILLATOR_OUTPUT_EN = OFF;

    //Setup direction of pinouts
    SWITCH_ONE_DIR = OUTPUT;
    SWITCH_TWO_DIR = OUTPUT;
    TRANSMIT_EN_DIR = OUTPUT;
    POWER_DETECTION_DIR = INPUT;
    TRANSMIT_CAP_DIR = OUTPUT;

    _INT0IE = OFF;
    
    while(1){
    
        DSCONbits.RELEASE = 0;   
        
        //Start charging transmit cap
        TRANSMIT_CAP_DIR = OUTPUT;
        TRANSMIT_CAP_SET = ON;
        
        //Disconnect data coil
        TRANSMIT_EN_SET = OFF;
        
        
        //Check for power
        while(1){
            SWITCH_ONE_SET = CLOSED;    //Close S1
            
            //wait for a little to charge
            asm("NOP");
            asm("NOP");
            asm("NOP");
            asm("NOP");
            asm("NOP");
            
            if(POWER_DETECTION_READ == ON){ //If cap was charged
               SWITCH_ONE_SET = OPEN; //Open S1
               SWITCH_TWO_SET = CLOSED; //Close S2 to drain cap
               asm("NOP");
               asm("NOP");
               SWITCH_TWO_SET = OPEN; //Open S2
            } else {                //There was no power detected
                
                TRANSMIT_CAP_DIR = HIGH_IMPEDANCE;
                TRANSMIT_EN_SET = ON; //Connect transmit antenna    
                break;              //Ready to transmit
            }
        }
        
        SWITCH_ONE_SET = OPEN; //Open S1
        SWITCH_TWO_SET = CLOSED; //Close S2 to drain cap
        
       __delay_ms(300);
        
        //Transmit Data
        
       //Not sure what this is, check to see if deletion is possible
        TRISBbits.TRISB15 = 0;
        ONE();
        hex4();
        hex8();
        hex6();
        hex5();
        hex6();
        hexC();
        hex6();
        hexC();
        hex6();
        hexF();

        TRANSMIT_EN_SET = OFF;
        
        //Save data in DSGPR0, DSGPR1
        
        SWITCH_ONE_SET = CLOSED; //Close S1
        SWITCH_TWO_SET = OPEN; //Open S2
        
        
        //Setup INT0
        _INT0EP = 0;    //Trigger on rising edge
        _INT0IF = 0;    //Reset interrupt flag
        _INT0IE = ON;    //Enable interrupt 
        
        //Go to deep sleep
        DSCONbits.DSEN = 1;
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("PWRSAV #0");
        
        DSCONbits.RELEASE = 0;
        RCONbits.DPSLP = 0;
        _INT0IE = OFF;
        
        
//        TRISBbits.TRISB1 = 0;
//        LATBbits.LATB1 = 1;
//        __delay_ms(1000);
//        TRISBbits.TRISB1 = 1;
//        
//                
//        ONE();
//        hexF();
//        hex0();
//        hexF();
//        hex0();
//        hexF();
//        hex0();
//        hexF();
//        hex0();
//        __delay_ms(3000)
        
        
        
        //asm("goto start");    
    }
    
    
    return 0;
}

