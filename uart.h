#include <stdio.h>
#include <stdlib.h>

#include "variables.h"
#include "timer.h"
#include "movement.h"

void initUART1(void);

void __attribute__((__interrupt__)) _U1RXInterrupt(void);

void WriteUART1(unsigned int data);

void WriteStringToUART1(unsigned char * str);