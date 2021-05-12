#include "timer.h"

#define M1_1 LATBbits.LATB3 
#define M1_2 LATBbits.LATB4 
#define M2_1 LATBbits.LATB5 
#define M2_2 LATBbits.LATB6 

// extern unsigned int fwd; //signal za kretanje napred
// extern unsigned int bck; //signal za kretanje nazad
// extern unsigned int left; //signal za skretanje levo
// extern unsigned int right;//signal za skretanje desno
// extern unsigned int fwd_left;
// extern unsigned int fwd_right;
// extern unsigned int distance_fwd;
// extern unsigned int distance_right;

void check_direction();

void stop();

void check_obstacles();