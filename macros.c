
#include "xc.h"
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include "macros.h"
#include "libpic30.h"

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
           })

#define ONE(void) ({\
        REFOCONbits.ROEN = 1;\
        asm("nop");\
        asm("nop");\
        asm("nop");\
        REFOCONbits.ROEN = 0;\
           })
    
    




