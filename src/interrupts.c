#include <xc.h>
#include "global.h"

void _ISR _T1Interrupt(void)
{
    CalculateSpeed(&motor_dx);
    CalculateSpeed(&motor_sx);
    
    ComputePID(&fooPID_dx,&motor_dx);
    ComputePID(&fooPID_sx,&motor_sx);
    
    SetMotorParameter();

    IFS0bits.T1IF = 0;    /* Clear Timer interrupt flag */

}

void _ISR _IC1Interrupt(void)
{
    unsigned int t1,t2;
    t1=IC1BUF;
    t2=IC1BUF;
    IFS0bits.IC1IF=0;
    if(t2>t1)
        motor_dx.enc.timePeriod = t2-t1;
    else
        motor_dx.enc.timePeriod = (65535 - t1) + t2;
    motor_dx.enc.count++;
}

void _ISR _IC2Interrupt(void)
{
    unsigned int t1,t2;
    t1=IC2BUF;
    t2=IC2BUF;
    IFS0bits.IC2IF=0;
    if(t2>t1)
        motor_sx.enc.timePeriod = t2-t1;
    else
        motor_sx.enc.timePeriod = (65535 - t1) + t2;
    motor_sx.enc.count++;
}

void _ISR _U1TXInterrupt(void)
{  
   IFS0bits.U1TXIF = 0;
} 

void __attribute__((__interrupt__)) _U1RXInterrupt(void)
{
    if (U1STAbits.FERR == 1)
    {
        U1STAbits.FERR = 0;
    } 
    if(U1STAbits.OERR == 1)
    {
        U1STAbits.OERR = 0;
    }
    /* Read the receive buffer till atleast one or more character can be read */ 
    while( DataRdyUART1())
    {
        if (!buffWrite)
        {
            consoleBuffer[buffWrite] = ReadUART1();
            if (consoleBuffer[buffWrite] == STX)
                buffWrite++;
                //frameOk = 1;
        }
        else
        {
            consoleBuffer[buffWrite] = ReadUART1();
            if (consoleBuffer[buffWrite] > 127 || buffWrite > 7)
            {
                //errore
                buffWrite = 0;
                siFrameReady = 0;
            }
            if((consoleBuffer[buffWrite]==ETX) && (buffWrite==2 || buffWrite==7))
            {
                if (buffWrite == 2)
                {
                    // 3 Bytes  ex: < STX "C" ETX >
                buffWrite = 0;
                siFrameReady = 1;
                command_type = 0;
                return;
                }
                else if (buffWrite == 7)
                {
                    // 6 Bytes  ex: < STX "200" "180" ETX >
                buffWrite = 0;
                siFrameReady = 1;
                command_type = 1;
                return;
                }
            }    
            buffWrite++;
        }
        /*
		consoleBuffer[buffWrite++] = ReadUART();
        if (buffWrite == 26)
        {
            buffWrite = 0;
            siFrameReady = 1;
        }
        */
    } 
    IFS0bits.U1RXIF = 0;
} 