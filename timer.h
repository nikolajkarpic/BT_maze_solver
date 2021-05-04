#include<p30fxxxx.h>

#define TMR1_period 10 /*  Fosc = 10MHz,
					          //1/Fosc = 0.1us !!!, 0.1us * 10 = 1us  */
#define TMR3_period 58800 /*  Fosc = 10MHz,
					          //1/Fosc = 0.1us !!!, 0.1us * 65000 = 6.5ms  */

#define servo_period 780 //sa preskalerom od 256 ova vrednost podesava frekvenciju pwm-a da bude 50Hz
#define motor_speed OC4RS //output compare na pinu RD3

//unsigned int stoperica = 0;

void set_speed(unsigned int percent);

void Init_T1(void);

void Init_T2(void);

void Init_T3(void);

void __attribute__ ((__interrupt__)) _T1Interrupt(void);

void __attribute__ ((__interrupt__)) _T2Interrupt(void);

void __attribute__ ((__interrupt__)) _T3Interrupt(void);

void delay_us (int vreme);

unsigned int ultrasonic_sensor_right();

unsigned int ultrasonic_sensor_fwd();