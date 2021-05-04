#include "timer.h"


void Init_T1(void)
{
	TMR1 = 0;
	PR1 = TMR1_period;
	
	T1CONbits.TCS = 0; // 0 = Internal clock (FOSC/4)
	//IPC1bits.T1IP = 3 // T1 interrupt pririty (0-7)
	//SRbits.IPL = 3; // CPU interrupt priority is 3(11)
	IFS0bits.T1IF = 0; // clear interrupt flag
	IEC0bits.T1IE = 1; // enable interrupt

	T1CONbits.TON = 1; // T1 on 
}

void Init_T2(void)
{
        PR2=servo_period;//odredjuje frekvenciju po formuli
        T2CONbits.TCKPS=0b11; //preskaler 1:256
        //IFS0bits.T2IF = 0; // clear interrupt flag
        //IEC0bits.T2IE = 1; // enable interrupt
        T2CONbits.TON=1;//ukljucujemo timer koji koristi
        
        OC4RS=20;//postavimo pwm
        OC4R=1000;//inicijalni pwm pri paljenju samo
        OC4CONbits.OCM = 0b110; //output ccompare mudule for pwm
        OC4RS = 0;
        
        IFS0bits.T2IF = 0; // clear interrupt flag
        IEC0bits.T2IE = 1; // enable interrupt
        
}

void Init_T3(void)
{
	TMR3 = 0;
	PR3 = TMR3_period;
	
	T3CONbits.TCS = 0; // 0 = Internal clock (FOSC/4)
	//IPC1bits.T3IP = 3 // T3 interrupt pririty (0-7)
	//SRbits.IPL = 3; // CPU interrupt priority is 3(11)
	IFS0bits.T3IF = 0; // clear interrupt flag
	IEC0bits.T3IE = 1; // enable interrupt

	T3CONbits.TON = 1; // T3 on 
}

void __attribute__ ((__interrupt__)) _T1Interrupt(void){//svakih 1us
    
    TMR1 =0;
    stoperica++;
    IFS0bits.T1IF = 0; 
    
}

void __attribute__ ((__interrupt__)) _T2Interrupt(void) // svakih 20ms
{//tajmer 2 radi na frekvenciji od 50Hz
    TMR2 = 0;
    IFS0bits.T2IF = 0; 
       
}
unsigned int tmr3_counter = 0;
void __attribute__ ((__interrupt__)) _T3Interrupt(void){ //svakih 6.5ms
    
    TMR3 =0;
    
    tmr3_counter++;
    IFS0bits.T3IF = 0; 
}

void delay_us (int vreme)//funkcija za kasnjenje u milisekundama
	{
        T1CONbits.TON = 1;
		stoperica = 0;
		while(stoperica < vreme);
        T1CONbits.TON = 0;
	}

    unsigned int ultrasonic_sensor_right(){
    unsigned int distance;
    
    TMR3=0;
    LATBbits.LATB0=1;//triger=1
    delay_us(10);
    LATBbits.LATB0 = 0;
    tmr3_counter = 0;
    while(!PORTBbits.RB1);
    T3CONbits.TON = 1;
    while(PORTBbits.RB1);
    T3CONbits.TON = 0;
    
    return TMR3/588 + tmr3_counter*100;
}

unsigned int ultrasonic_sensor_right(){
    unsigned int distance;
    
    TMR3=0;
    LATBbits.LATB0=1;//triger=1
    delay_us(10);
    LATBbits.LATB0 = 0;
    tmr3_counter = 0;
    while(!PORTBbits.RB1);
    T3CONbits.TON = 1;
    while(PORTBbits.RB1);
    T3CONbits.TON = 0;
    
    return TMR3/588 + tmr3_counter*100;
}

unsigned int ultrasonic_sensor_fwd(){
    
    TMR3=0;
    trig2 = 1;
    delay_us(10);
    trig2 = 0;
    tmr3_counter = 0;
    while(!echo2);
    T3CONbits.TON = 1;
    while(echo2);
    T3CONbits.TON = 0;
    
    return TMR3/588+100*tmr3_counter;
}
