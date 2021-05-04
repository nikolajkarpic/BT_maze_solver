/*
 * File:   main.c
 * Author: Nikolaj
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


#define trig2 LATDbits.LATD0 
#define echo2 PORTDbits.RD1
#define trig1 LATBbits.LATB0
#define echo1 PORTBbits.RB1

#define M1_1 LATBbits.LATB3 
#define M1_2 LATBbits.LATB4 
#define M2_1 LATBbits.LATB5 
#define M2_2 LATBbits.LATB6 

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

unsigned int brojac_desno = 0, br;


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
