#include "movement.h"



void check_direction(){ //kontrola H mosta za smer motora

    
    if(fwd == 1){ //1010 je za smer napred
        M1_1 = 1;
        M1_2 = 0;
        M2_1 = 1;
        M2_2 = 0;
        
        return 0;
    }
    if(bck == 1){// 0101 je smer za nazad
        M1_1 = 0;
        M1_2 = 1;
        M2_1 = 0;
        M2_2 = 1;
        
        return 0;
    }
    
    if(left == 1){//Smer za levo je 1001
        M1_1 = 1;
        M1_2 = 0;
        M2_1 = 0;
        M2_2 = 1;
        
        return 0;
    }if(right == 1){ //smer za desno je 0110
        M1_1 = 0;
        M1_2 = 1;
        M2_1 = 1;
        M2_2 = 0;
        
        return 0;
    }if(fwd_right == 1){
        M1_1 = 0;
        M1_2 = 0;
        M2_1 = 1;
        M2_2 = 0;
        
        return 0;
    }if(fwd_left == 1){
        M1_1 = 1;
        M1_2 = 0;
        M2_1 = 0;
        M2_2 = 0;
        
        return 0;
    }
    
    M1_1 = 0;//motori su ugaseni za 0000
    M1_2 = 0;
    M2_1 = 0;
    M2_2 = 0;
}


void stop(){
    fwd = 0;
    bck = 0;
    left = 0;
    right = 0;
    fwd_left = 0;
    fwd_right = 0;
}

void check_obstacles(){
    
    
    char buffer[20];
    
    distance_fwd = ultrasonic_sensor_fwd();
    distance_right = ultrasonic_sensor_right();

    sprintf(buffer, "desno: %dcm\n", distance_right);
    WriteStringToUART1(buffer);

    sprintf(buffer, "napred: %dcm\n", distance_fwd);
    //WriteStringToUART1(buffer);

    
   // if(distance_right == 27 || distance_right == 28) fwd_right = 1;
    
    
}