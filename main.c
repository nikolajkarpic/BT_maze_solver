/*
 * File:   main.c
 * Author: mendj
 *
 * Created on April 27, 2021, 2:24 PM
 */


// #include <stdio.h>
// #include <stdlib.h>
//#include<p30fxxxx.h>

#include "timer.h"
#include "uart.h"
#include "movement.h"
_FOSC(CSW_FSCM_OFF & XT_PLL4);//instruction takt je isti kao i kristal
_FWDT(WDT_OFF);

// #define TMR1_period 10 /*  Fosc = 10MHz,
// 					          1/Fosc = 0.1us !!!, 0.1us * 10 = 1us  */
// #define TMR3_period 58800 /*  Fosc = 10MHz,
// 					          1/Fosc = 0.1us !!!, 0.1us * 65000 = 6.5ms  */
#define trig2 LATDbits.LATD0 
#define echo2 PORTDbits.RD1
#define trig1 LATBbits.LATB0
#define echo1 PORTBbits.RB1

#define M1_1 LATBbits.LATB3 
#define M1_2 LATBbits.LATB4 
#define M2_1 LATBbits.LATB5 
#define M2_2 LATBbits.LATB6 

// #define servo_period 780 //sa preskalerom od 256 ova vrednost podesava frekvenciju pwm-a da bude 50Hz
// #define motor_speed OC4RS //output compare na pinu RD3

//unsigned int stoperica = 0;
// unsigned char tempRX;

unsigned int distance_fwd;
unsigned int distance_right;
unsigned int turning_threshold = 30;
unsigned int turning_time_left = 65;
unsigned int turning_time_right = 10;

unsigned int fwd = 0; //signal za kretanje napred
unsigned int bck = 0; //signal za kretanje nazad
unsigned int left = 0; //signal za skretanje levo
unsigned int right = 0;//signal za skretanje desno
unsigned int fwd_left = 0;
unsigned int fwd_right = 0;

unsigned int prefered_distance_fwd = 10;
unsigned int prefered_distance_right = 17;
unsigned int hysteresis_fwd = 5;
unsigned int hysteresis_right = 1;

enum tank_state{ idle_s, fwd_s, left_s, right_s, bck_s} current_state;
tank_state next_state;

// void Init_T1(void)
// {
// 	TMR1 = 0;
// 	PR1 = TMR1_period;
	
// 	T1CONbits.TCS = 0; // 0 = Internal clock (FOSC/4)
// 	//IPC1bits.T1IP = 3 // T1 interrupt pririty (0-7)
// 	//SRbits.IPL = 3; // CPU interrupt priority is 3(11)
// 	IFS0bits.T1IF = 0; // clear interrupt flag
// 	IEC0bits.T1IE = 1; // enable interrupt

// 	T1CONbits.TON = 1; // T1 on 
// }

// void Init_T2(void)
// {
//         PR2=servo_period;//odredjuje frekvenciju po formuli
//         T2CONbits.TCKPS=0b11; //preskaler 1:256
//         //IFS0bits.T2IF = 0; // clear interrupt flag
//         //IEC0bits.T2IE = 1; // enable interrupt
//         T2CONbits.TON=1;//ukljucujemo timer koji koristi
        
//         OC4RS=20;//postavimo pwm
//         OC4R=1000;//inicijalni pwm pri paljenju samo
//         OC4CONbits.OCM = 0b110; //output ccompare mudule for pwm
//         OC4RS = 0;
        
//         IFS0bits.T2IF = 0; // clear interrupt flag
//         IEC0bits.T2IE = 1; // enable interrupt
        
// }

// void Init_T3(void)
// {
// 	TMR3 = 0;
// 	PR3 = TMR3_period;
	
// 	T3CONbits.TCS = 0; // 0 = Internal clock (FOSC/4)
// 	//IPC1bits.T3IP = 3 // T3 interrupt pririty (0-7)
// 	//SRbits.IPL = 3; // CPU interrupt priority is 3(11)
// 	IFS0bits.T3IF = 0; // clear interrupt flag
// 	IEC0bits.T3IE = 1; // enable interrupt

// 	T3CONbits.TON = 1; // T3 on 
// }

// void __attribute__ ((__interrupt__)) _T1Interrupt(void){//svakih 1us
    
//     TMR1 =0;
//     stoperica++;
//     IFS0bits.T1IF = 0; 
    
// }

// void __attribute__ ((__interrupt__)) _T2Interrupt(void) // svakih 20ms
// {//tajmer 2 radi na frekvenciji od 50Hz
//     TMR2 = 0;
//     IFS0bits.T2IF = 0; 
       
// }
// unsigned int tmr3_counter = 0;
// void __attribute__ ((__interrupt__)) _T3Interrupt(void){ //svakih 6.5ms
    
//     TMR3 =0;
    
//     tmr3_counter++;
//     IFS0bits.T3IF = 0; 
// }

// void set_speed(unsigned int percent){
//     unsigned int temp = 780*percent;
//     temp/=100;
//     motor_speed = temp;
// }

// void delay_us (int vreme)//funkcija za kasnjenje u milisekundama
// 	{
//         T1CONbits.TON = 1;
// 		stoperica = 0;
// 		while(stoperica < vreme);
//         T1CONbits.TON = 0;
// 	}

// unsigned int ultrasonic_sensor_right(){
//     unsigned int distance;
    
//     TMR3=0;
//     LATBbits.LATB0=1;//triger=1
//     delay_us(10);
//     LATBbits.LATB0 = 0;
//     tmr3_counter = 0;
//     while(!PORTBbits.RB1);
//     T3CONbits.TON = 1;
//     while(PORTBbits.RB1);
//     T3CONbits.TON = 0;
    
//     return TMR3/588 + tmr3_counter*100;
// }

// unsigned int ultrasonic_sensor_fwd(){
    
//     TMR3=0;
//     trig2 = 1;
//     delay_us(10);
//     trig2 = 0;
//     tmr3_counter = 0;
//     while(!echo2);
//     T3CONbits.TON = 1;
//     while(echo2);
//     T3CONbits.TON = 0;
    
//     return TMR3/588+100*tmr3_counter;
// }

// void initUART1(void)
// {
//     U1BRG=0x0040;//ovim odredjujemo baudrate

//     U1MODEbits.ALTIO=1;//biramo koje pinove koristimo za komunikaciju osnovne ili alternativne

//     IEC0bits.U1RXIE=1;//omogucavamo rx1 interupt

//     U1STA&=0xfffc;

//     U1MODEbits.UARTEN=1;//ukljucujemo ovaj modul

//     U1STAbits.UTXEN=1;//ukljucujemo predaju
// }

// void __attribute__((__interrupt__)) _U1RXInterrupt(void) 
// {
//     IFS0bits.U1RXIF = 0;
//     tempRX=U1RXREG;

// } 

// /*********************************************************************
// * Ime funkcije      : WriteUART1                                     *
// * Opis              : Funkcija upisuje podatak u registar 			 *
// * 					  za slanje podataka U1TXREG,   				 *
// * Parametri         : unsigned int data-podatak koji treba poslati   *
// * Povratna vrednost : Nema                                           *
// *********************************************************************/

// void WriteUART1(unsigned int data)
// {
// 	  while(!U1STAbits.TRMT);

//     if(U1MODEbits.PDSEL == 3)
//         U1TXREG = data;
//     else
//         U1TXREG = data & 0xFF;
// }

// void WriteStringToUART1(unsigned char * str){
//     unsigned int k = 0;
//     while(*(str+k) != 0x00){
//         WriteUART1(*(str+k));
//         k++;
//     }
// }
// void check_direction(){ //kontrola H mosta za smer motora

    
//     if(fwd == 1){ //1010 je za smer napred
//         M1_1 = 1;
//         M1_2 = 0;
//         M2_1 = 1;
//         M2_2 = 0;
        
//         return 0;
//     }
//     if(bck == 1){// 0101 je smer za nazad
//         M1_1 = 0;
//         M1_2 = 1;
//         M2_1 = 0;
//         M2_2 = 1;
        
//         return 0;
//     }
    
//     if(left == 1){//Smer za levo je 1001
//         M1_1 = 1;
//         M1_2 = 0;
//         M2_1 = 0;
//         M2_2 = 1;
        
//         return 0;
//     }if(right == 1){ //smer za desno je 0110
//         M1_1 = 0;
//         M1_2 = 1;
//         M2_1 = 1;
//         M2_2 = 0;
        
//         return 0;
//     }if(fwd_right == 1){
//         M1_1 = 0;
//         M1_2 = 0;
//         M2_1 = 1;
//         M2_2 = 0;
        
//         return 0;
//     }if(fwd_left == 1){
//         M1_1 = 1;
//         M1_2 = 0;
//         M2_1 = 0;
//         M2_2 = 0;
        
//         return 0;
//     }
    
//     M1_1 = 0;//motori su ugaseni za 0000
//     M1_2 = 0;
//     M2_1 = 0;
//     M2_2 = 0;
// }
unsigned int brojac_desno = 0, br;

// void stop(){
//     fwd = 0;
//     bck = 0;
//     left = 0;
//     right = 0;
//     fwd_left = 0;
//     fwd_right = 0;
// }

// void check_obstacles(){
    
    
//     char buffer[20];
    
//     distance_fwd = ultrasonic_sensor_fwd();
//     distance_right = ultrasonic_sensor_right();

//     sprintf(buffer, "desno: %dcm\n", distance_right);
//     WriteStringToUART1(buffer);

//     sprintf(buffer, "napred: %dcm\n", distance_fwd);
//     //WriteStringToUART1(buffer);

    
//    // if(distance_right == 27 || distance_right == 28) fwd_right = 1;
    
    
// }

void print_state(){
    switch(current_state){
        case idle_s:
            WriteStringToUART1("State: IDLE\n");
            break;
        case fwd_s:
            WriteStringToUART1("State: FWD\n");
            break;
        case bck_s:
            WriteStringToUART1("State: BCK\n");
            break;
        case left_s:
            WriteStringToUART1("State: LEFT\n");
            break;
        case right_s:
            WriteStringToUART1("State: RIGHT\n");
            break;
            
    }
}
unsigned int br = 0;

int main(void) {
    
    ADPCFGbits.PCFG0=1;
    ADPCFGbits.PCFG1=1;
    ADPCFGbits.PCFG3=1;//kazemo da nije analogni vec digitalni pin
    ADPCFGbits.PCFG4=1;//kazemo da nije analogni vec digitalni pin
    ADPCFGbits.PCFG5=1;//kazemo da nije analogni vec digitalni pin
    ADPCFGbits.PCFG6=1;//kazemo da nije analogni vec digitalni pin
    
    TRISBbits.TRISB0=0;//triger1
    TRISBbits.TRISB1=1;//echo1
    TRISBbits.TRISB3=0;//konfigurisemo kao izlaz
    TRISBbits.TRISB4=0;//konfigurisemo kao izlaz
    TRISBbits.TRISB5=0;//konfigurisemo kao izlaz
    TRISBbits.TRISB6=0;//konfigurisemo kao izlaz
    
    TRISDbits.TRISD0 = 0;//triger2
    TRISDbits.TRISD1 = 1;//echo2
    
    LATBbits.LATB3 = 0; //M1_1
    LATBbits.LATB4 = 0; //M1_2
    LATBbits.LATB5 = 0; //M2_1
    LATBbits.LATB6 = 0; //M2_2
            
    state = idle_s;
    
    Init_T1();
    Init_T2();
    Init_T3();
    initUART1();
    set_speed(15);//brzina je 60%
    unsigned int i;
    
    while(1){
        check_obstacles();
        current_state = next_state;

        switch (current_state){
            case idle_s:
                fwd = 1;
                if (distance_right <= prefered_distance_right + hysteresis_right){
                    if (distance_right >= prefered_distance_right - hysteresis_right ){
                        stop();
                        fwd = 1;
                        next_state = fwd_s;
                        break;
                    }else {
                        stop();
                        left = 1;
                        //break;
                    }
                } else {
                    stop();
                    right = 1;
                    //break;
                }
                break;
            case fwd_s:
                fwd = 1;
                if (distance_right <= prefered_distance_right + hysteresis_right){
                    //idi desno
                    if (distance_right >= prefered_distance_right - hysteresis_right ){
                        fwd = 1;
                        
                        //state = fwd_s;
                    }else {
                        stop();
                        left = 1;
                    }
                } else {
                    stop();
                    right = 1;
                }
                if (distance_fwd <= prefered_distance_fwd - hysteresis_fwd){
                    //if(distance_fwd >= prefered_distance_fwd - hysteresis_fwd){
                        next_state = left_s; 
                        break;
                    /*}else{
                    stop();
                    state = bck_s;
                    WriteStringToUART1("USAO!!!\n");
                    break;
                    }*/
                }
                if (distance_right >= turning_threshold){
                    next_state = right_s;
                    break;
                }
                break;
            case left_s:
                stop();
                left = 1;// treba fiksno vreme za rotaciju od 90 stepeni porm. unsigned int turning_time_left;
                check_direction();
                while(!(distance_right <= prefered_distance_right + hysteresis_right && distance_right >= prefered_distance_right - hysteresis_right)){
                //for (i = 0; i<turning_time_left; i++){
                    delay_us(10000);
                }
                //stop();
                // fwd = 1;
                // check_direction();
                // for (i = 0; i<turning_time_right+10; i++){
                //     delay_us(10000);
                // }
                next_state = fwd_s;
                break;
            case right_s:
                stop();
                fwd = 1;
                check_direction();
                for (i = 0; i<turning_time_right+10; i++){
                    delay_us(10000);
                }
                stop();
                fwd_right = 1;// treba fiksno vreme za rotaciju od 90 stepeni porm. unsigned int turning_time_right;
                check_direction();
                for (i = 0; i<turning_time_right; i++){
                    delay_us(10000);
                }
                next_state = fwd_s;
                break;
            case bck_s:
                stop();
                bck = 1;
                if(distance_fwd >= prefered_distance_fwd - hysteresis_fwd){
                    next_state = fwd_s;
                    break;
                }
                break;
            default:
                break;
    
        }
        check_direction();
        
        for(br = 0;br<10;br++){
            delay_us(10000);
        }
        
        print_state();
    }
    return 0;
}
