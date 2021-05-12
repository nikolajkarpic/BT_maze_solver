#include <stdio.h>
#include <stdlib.h>

#include<p30fxxxx.h>

//extern unsigned char tempRX;

void initUART1(void);

void __attribute__((__interrupt__)) _U1RXInterrupt(void);

void WriteUART1(unsigned int data);

void WriteStringToUART1(unsigned char * str);