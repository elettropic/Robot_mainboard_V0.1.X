/*
 * File:   main.c
 * Author: bonettil
 *
 * Created on 17 marzo 2016, 9.31
 */

#define FOSC    (80000000ULL)
#define FCY     (FOSC/2)
 
#include <xc.h>
#include <stdio.h>
#include "declarations.h"


int main(void) 
{
    ConfigurePeripheral();
    PID_setup();
    
    while(1)
	{
        
        serial_process();

        //printf("Speed_act %.2f, Speed_req %.2f, Tick_cnt %d, Count %d\n\r",speed_act,speed_req,timePeriod_left,count_left);
        __delay_ms(25);
	}
    return 0;
}
