


#include "adc.h"



void adcInit(){
    
    AD1PCFG = 0xFFFB; // AN2 as analog, all other pins are digital
    AD1CON1 = 0x00E0; // SAMP bit = 0 ends sampling and starts converting
    AD1CHS = 0x0002; // Connect AN2 as S/H+ input
    // in this example AN2 is the input
    AD1CSSL = 0;
    AD1CON3 = 0x0202; // Manual Sample, Tad = 3Tcy
    AD1CON2 = 0;
    AD1CON1bits.ADON = 1; // turn ADC ON
    
}


int getADC(){
    
    AD1CON1bits.SAMP = 1; // start sampling, then after 31Tad go to conversion
    while (!AD1CON1bits.DONE){}; // conversion done?
    return ADC1BUF0; 
    
}