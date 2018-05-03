/* 
 * File:   macros.h
 * Author: bazle
 *
 * Created on April 3, 2018, 1:50 PM
 */

#ifndef MACROS_H
#define	MACROS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    
 #define FCY 31400UL       // instruction cycle Hrtz

// libpic30 delays 
#define __delay_ms(d) \
  { __delay32( (unsigned long) (((unsigned long long) d)*(FCY)/1000ULL)); }
#define __delay_us(d) \
  { __delay32( (unsigned long) (((unsigned long long) d)*(FCY)/1000000ULL)); }


#define ZERO(void) ({\
        asm("nop");\
        asm("nop");\
        asm("nop");\
        asm("nop");\
        asm("nop");\
        asm("nop");\
           })

#define ONE(void) ({\
        REFOCONbits.ROEN = 1;\
        asm("nop");\
        asm("nop");\
        asm("nop");\
        REFOCONbits.ROEN = 0;\
           })

static inline void hexF(){
        __delay_us(100); 
        ONE();
        __delay_us(100); 
        ONE();
        __delay_us(100);
        ONE();
        __delay_us(100);
        ONE();
}

static inline void hexE(){
        __delay_us(100); 
        ONE();
        __delay_us(100); 
        ONE();
        __delay_us(100);
        ONE();
        __delay_us(100);
        ZERO();
}

static inline void hexD(){
        __delay_us(100); 
        ONE();
        __delay_us(100); 
        ONE();
        __delay_us(100);
        ZERO();
        __delay_us(100);
        ONE();
}

static inline void hexC(){
        __delay_us(100); 
        ONE();
        __delay_us(100); 
        ONE();
        __delay_us(100);
        ZERO();
        __delay_us(100);
        ZERO();
}

static inline void hexB(){
        __delay_us(100); 
        ONE();
        __delay_us(100); 
        ZERO();
        __delay_us(100);
        ONE();
        __delay_us(100);
        ONE();
}

static inline void hexA(){
        __delay_us(100); 
        ONE();
        __delay_us(100); 
        ZERO();
        __delay_us(100);
        ONE();
        __delay_us(100);
        ZERO();
}



static inline void hex8(){
        __delay_us(100); 
        ONE();
        __delay_us(100); 
        ZERO();
        __delay_us(100);
        ZERO();
        __delay_us(100);
        ZERO();
}

static inline void hex7(){
        __delay_us(100); 
        ZERO();
        __delay_us(100); 
        ONE();
        __delay_us(100);
        ONE();
        __delay_us(100);
        ONE();
}

static inline void hex6(){
        __delay_us(100); 
        ZERO();
        __delay_us(100); 
        ONE();
        __delay_us(100);
        ONE();
        __delay_us(100);
        ZERO();
}



static inline void hex4(){
        __delay_us(100); 
        ZERO();
        __delay_us(100); 
        ONE();
        __delay_us(100);
        ZERO();
        __delay_us(100);
        ZERO();
}

static inline void hex3(){
        __delay_us(100); 
        ZERO();
        __delay_us(100); 
        ZERO();
        __delay_us(100);
        ONE();
        __delay_us(100);
        ONE();
}

static inline void hex2(){
        __delay_us(100); 
        ZERO();
        __delay_us(100); 
        ZERO();
        __delay_us(100);
        ONE();
        __delay_us(100);
        ZERO();
}

static inline void hex1(){
        __delay_us(100); 
        ZERO();
        __delay_us(100); 
        ZERO();
        __delay_us(100);
        ZERO();
        __delay_us(100);
        ONE();
}

static inline void hex0(){
        __delay_us(100); 
        ZERO();
        __delay_us(100); 
        ZERO();
        __delay_us(100);
        ZERO();
        __delay_us(100);
        ZERO();
}


static inline void hex9(){
        __delay_us(100); 
        ONE();
        __delay_us(100); 
        ZERO();
        __delay_us(100);
        ZERO();
        __delay_us(100);
        ONE();
}

static inline void hex5(){
        __delay_us(100); 
        ZERO();
        __delay_us(50); 
        ONE();
        __delay_us(100);
        ZERO();
        __delay_us(50);
        ONE();
}
 

#ifdef	__cplusplus
}
#endif

#endif	/* MACROS_H */

