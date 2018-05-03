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


#define S1Pin RB14
#define S2Pin RB13
#define PowerCheckPin RA6

#define FCY 31400UL       // instruction cycle Hrtz

// libpic30 delays 
#define __delay_ms(d) \
  { __delay32( (unsigned long) (((unsigned long long) d)*(FCY)/1000ULL)); }
#define __delay_us(d) \
  { __delay32( (unsigned long) (((unsigned long long) d)*(FCY)/1000000ULL)); }

#define ONE(void) ({\
        REFOCONbits.ROEN = 1;\
        asm("nop");\
        asm("nop");\
        asm("nop");\
        REFOCONbits.ROEN = 0;\
           })




int main(void) {
    
    //Setup oscillator output for transmission
    TRISBbits.TRISB15 = 0;
    REFOCONbits.ROSSLP = 0;
    REFOCONbits.ROSEL = 0;
    REFOCONbits.RODIV = 0b0001;
    REFOCONbits.ROEN = 0;

    //Setup switch controls
    TRISBbits.TRISB14 = 0;
    TRISBbits.TRISB13 = 0;
    TRISBbits.TRISB7 = 1;
    TRISBbits.TRISB8 = 0;
    
    TRISBbits.TRISB1 = 0;
    _INT0IE = 0;
    
    while(1){
    
//        DSCONbits.RELEASE = 0;   
//        
//        //Start charging transmit cap
//        TRISBbits.TRISB1 = 0;
//        LATBbits.LATB1 = 1;
//        
//        //Disconnect data coil
//        LATBbits.LATB8 = 0;
//        
//        
//        //Check for power
//        while(1){
//            LATBbits.LATB14 = 1;    //Close S1
//            //wait for a little to charge
//            asm("NOP");
//            asm("NOP");
//            asm("NOP");
//            asm("NOP");
//            asm("NOP");
//            
//            if(PORTBbits.RB7 == 1){ //If cap was charged
//               LATBbits.LATB14 = 0; //Open S1
//               LATBbits.LATB13 = 1; //Close S2 to drain cap
//               LATBbits.LATB13 = 0; //Open S2
//            } else {                //There was no power detected
//                //LATBbits.LATB1 = 0;
//                TRISBbits.TRISB1 = 1;
//                LATBbits.LATB8 = 1; //Connect transmit antenna
//                
//                break;              //Ready to transmit
//            }
//        }
//        
//        LATBbits.LATB14 = 0; //Open S1
//        LATBbits.LATB13 = 1; //Close S2 to drain cap
//        
//        
//        //Transmit Data
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
//
//        
//        //Save data in DSGPR0, DSGPR1
//        
//        LATBbits.LATB14 = 1; //Close S1
//        LATBbits.LATB13 = 0; //Open S2
//        
//        
//        //Setup INT0
//        _INT0EP = 0;    //Trigger on rising edge
//        _INT0IF = 0;    //Reset interrupt flag
//        _INT0IE = 1;    //Enable interrupt 
//        
//        //Go to deep sleep
//        DSCONbits.DSEN = 1;
//        asm("NOP");
//        asm("NOP");
//        asm("NOP");
//        asm("PWRSAV #0");
//        
//        DSCONbits.RELEASE = 0;
//        RCONbits.DPSLP = 0;
//        _INT0IE = 0;
        
        
        TRISBbits.TRISB1 = 0;
        LATBbits.LATB1 = 1;
        __delay_ms(1000);
        TRISBbits.TRISB1 = 1;
        
                
        ONE();
        hexF();
        hex0();
        hexF();
        hex0();
        hexF();
        hex0();
        hexF();
        hex0();
        __delay_ms(3000)
        
        
        
        //asm("goto start");    
    }
    
    
    return 0;
}

