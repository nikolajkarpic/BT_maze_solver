#include "timer.h"
#include "uart.h"
#include "movement.h"

#define trig2 LATDbits.LATD0 
#define echo2 PORTDbits.RD1
#define trig1 LATBbits.LATB0
#define echo1 PORTBbits.RB1

#define M1_1 LATBbits.LATB3 
#define M1_2 LATBbits.LATB4 
#define M2_1 LATBbits.LATB5 
#define M2_2 LATBbits.LATB6 
