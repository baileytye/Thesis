/*
 * File:   main.c
 * Author: Bailey Tye
 *
 * Created on March 28, 2018, 11:53 AM
 * Last Edited: May 17, 2018
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
#include "adc.h"


/*-----------------------User Definitions------------------------------*/


#define BERTEST 0


/* PIN MAPPING
 * B15 - OSCILLATOR_OUTPUT - Output of internal oscillator, used to control the 
 *       data transmission.  
 * B14 - SWITCH_ONE - First switch in the power detection circuit
 * B13 - SWITCH_TWO - Second switch in the power detection circuit
 * B8 - TRANSMIT_EN - Switch to enable transmission on data to reader
 * B7 - POWER_DETECTION - Voltage reading from cap to determine if power was detected
 * B1 - REMOVED - TRANSMIT_CAP - Cap used as power source for data transmission - REMOVED
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

#define ADC_INPUT_DIR TRISBbits.TRISB0


//Removed
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
        asm("BSET 0x74F, #7");\
        asm("nop");\
        asm("nop");\
        asm("nop");\
        asm("BCLR 0x74F, #7");\
           })




int main(void) {
    
    DSCONbits.RELEASE = 0; 
    if(!RCONbits.DPSLP){
        DSGPR0 = 0;
    }
    RCONbits.DPSLP = 0;
    _INT0IE = OFF;

    //Setup oscillator output for transmission
    OSCILLATOR_OUTPUT_DIR = OUTPUT;
    REFOCONbits.ROSSLP = 0;
    REFOCONbits.ROSEL = 0;
    REFOCONbits.RODIV = 0b0001;
    OSCILLATOR_OUTPUT_EN = OFF;
    
    #ifdef BERTEST
    int state;
    #else
    ADC_INPUT_DIR = INPUT;
    #endif


    //Setup direction of pinouts
    SWITCH_ONE_DIR = OUTPUT;
    SWITCH_ONE_SET = CLOSED;
    SWITCH_TWO_DIR = OUTPUT;
    TRANSMIT_EN_DIR = OUTPUT;
    POWER_DETECTION_DIR = INPUT;
    //TRANSMIT_CAP_DIR = OUTPUT;

    

    
    //__delay_ms(100);
    
//    /****************TEST TIMING CODE*****************/
//    
//        //Start charging transmit cap
//        TRANSMIT_CAP_DIR = OUTPUT;
//        TRANSMIT_CAP_SET = ON;
//        __delay_ms(100);
//        
//        TRANSMIT_CAP_DIR = HIGH_IMPEDANCE;
//
//        DSGPR0 = 0xF0F0;
//        DSGPR1 = 0x5555;
//        transmitData();
//        __delay_ms(1000);
//        
//        
//    /****************END OF TEST TIMING****************/

    while(1){
          
        //Disconnect data coil
        TRANSMIT_EN_SET = OFF;
        
        //REMOVED - Start charging transmit cap
        //TRANSMIT_CAP_DIR = OUTPUT;
        //TRANSMIT_CAP_SET = ON;
        
        #ifdef BERTEST 
            //Get state from previous sleep
            state = ++DSGPR0;
            if(state >= 7){
                state = 1;
            }
        #else
            adcInit();  
            DSGPR0 = getADC();
            AD1CON1bits.ADON = 0;
        #endif
        
        //----Check for power, and wait till power turns off----//
        while(1){
            SWITCH_ONE_SET = CLOSED;    //Close S1
            
            //wait for a little to charge
            asm("NOP");
            asm("NOP");
            asm("NOP");
            asm("NOP");
            asm("NOP");
            
            if(POWER_DETECTION_READ == ON){     //If cap was charged
               SWITCH_ONE_SET = OPEN;           //Open S1
               SWITCH_TWO_SET = CLOSED;         //Close S2 to drain cap
               asm("NOP");
               asm("NOP");
               SWITCH_TWO_SET = OPEN;           //Open S2
            } else {                            //There was no power detected
                
                //TRANSMIT_CAP_DIR = HIGH_IMPEDANCE;
                TRANSMIT_EN_SET = ON;           //Connect transmit antenna    
                break;                          //Ready to transmit
            }
        }
        //--------------------------------------------------------//
        
        SWITCH_ONE_SET = OPEN; //Open S1
        SWITCH_TWO_SET = CLOSED; //Close S2 to drain cap
        
        //Wait for a second for the amp on reader to settle
        __delay_ms(15);
        

        /***************BER TEST CODE***************/        
        #ifdef BERTEST

        //Transmit Data with odd parity
        switch(state){
            case 1:
                //Transmit all zeroes 0x0000 0000 8
                DSGPR0 = 0x0000;
                DSGPR1 = 0x0000;
                transmitData();
                break;
            case 2:
                //Transmit all ones 0xFFFF FFFF 8
                DSGPR0 = 0xFFFF;
                DSGPR1 = 0xFFFF;   
                transmitData();
                break;
            case 3:
                //Transmit alternating 0x5555 5555 8
                DSGPR0 = 0x5555;
                DSGPR1 = 0x5555;
                transmitData();
                break;
            case 4:
                //Transmit group alternating 0xF0F0 F0F0 8
                DSGPR0 = 0xF0F0;
                DSGPR1 = 0xF0F0;
                transmitData();
                break;
            case 5:
                //Transmit first random word (open) 0x6F70 656E 8
                DSGPR0 = 0x6F70;
                DSGPR1 = 0x656E;
                transmitData();
                break;
            case 6:
                //Transmit second random data (!@#%) 0x2140 2325 0
                DSGPR0 = 0x2140;
                DSGPR1 = 0x2325;
                transmitData();
                break;
            default:
                //Transmit fail 0x4661 696C 8
                DSGPR0 = 0x4661;
                DSGPR1 = 0x696C;
                transmitData();
                break;
        }       
        DSGPR0 = state;
        /*********** END OF BER TEST CODE ************/
        #else

        DSGPR1 = 0x1111;
        transmitData();
        
        #endif

        TRANSMIT_EN_SET = OFF;
        
        //Save data in DSGPR0 075C, DSGPR1 075E
        
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

        
        
    }
    
    
    return 0;
}



void transmitData(){
    
    //Calculate parity
    int i, count = 0, b = 1;

    for(i = 0; i < 16; i++){
        if( DSGPR0 & (b << i) ){count++;}
    }
    b = 1;
    for(i = 0; i < 16; i++){
        if( DSGPR1 & (b << i) ){count++;}
    } 
    if((count % 2) == 0){
        asm("MOV #1, W7");
    }
    
    //Send start bit
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    
    //Send Data
    asm("BTSC DSGPR0, #15");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR0, #14");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR0, #13");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR0, #12");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR0, #11");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR0, #10");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR0, #9");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR0, #8");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR0, #7");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR0, #6");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR0, #5");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR0, #4");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR0, #3");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR0, #2");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR0, #1");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR0, #0");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    
    
    
    
    asm("BTSC DSGPR1, #15");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR1, #14");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR1, #13");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR1, #12");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR1, #11");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR1, #10");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR1, #9");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR1, #8");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR1, #7");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR1, #6");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR1, #5");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR1, #4");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR1, #3");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR1, #2");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR1, #1");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    asm("BTSC DSGPR1, #0");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    __delay_us(100);
    
    
    //Send parity
    asm("BTSC W7 , #0");
    asm("BSET 0x74F, #7");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("BCLR 0x74F, #7");
    
    
    return;
    
}

