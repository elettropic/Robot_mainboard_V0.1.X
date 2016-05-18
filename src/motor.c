#include <xc.h>
#include "global.h"


void PID_setup()
{
    
    fooPID_dx.abcCoefficients = &abcCoefficient_dx[0];      /*Set up pointer to derived coefficients */
    fooPID_dx.controlHistory = &controlHistory_dx[0];       /*Set up pointer to controller history samples */
    PIDInit(&fooPID_dx);                                    /*Clear the controler history and the controller output */
    
    fooPID_sx.abcCoefficients = &abcCoefficient_sx[0];    
    fooPID_sx.controlHistory = &controlHistory_sx[0];     
    PIDInit(&fooPID_sx);                               
    
	kCoeffs_dx[0] = Q15(kp);
	kCoeffs_dx[1] = Q15(ki);
	kCoeffs_dx[2] = Q15(kd);

    kCoeffs_sx[0] = Q15(kp);
	kCoeffs_sx[1] = Q15(ki);
	kCoeffs_sx[2] = Q15(kd);

    PIDCoeffCalc(&kCoeffs_dx[0], &fooPID_dx);               /*Derive the a,b, & c coefficients from the Kp, Ki & Kd */
    PIDCoeffCalc(&kCoeffs_sx[0], &fooPID_sx);
}

void CalculateSpeed (motor* motor_param)
{
    if (motor_param->enc.count_old == motor_param->enc.count)
    {
        motor_param->speed_actual = 0;
    }
    else
    {
        motor_param->speed_actual = (626162/motor_param->enc.timePeriod);
        motor_param->speed_actual /=4.26;
    }
    motor_param->enc.count_old = motor_param->enc.count;
}

void ComputePID (tPID* PID_param, motor* motor_param)
{
    PID_param->controlReference = Q15((float)motor_param->speed_reqest/(float)200);        
    if (motor_param->speed_reqest > 0)
        PID_param->measuredOutput =  Q15((float)motor_param->speed_actual/(float)200);                     
    else
        PID_param->measuredOutput =  Q15((float)motor_param->speed_actual/(float)-200);                     
    PID(PID_param);                               
    
    motor_param->PID_out = ((int) 2500* Fract2Float(PID_param->controlOutput));
}

void SetMotorParameter()
{
    if ((motor_dx.PID_out < 0)&&(motor_dx.speed_reqest < 0))// && (abs(outPID_1) > dead_zone))
    {
        IOCON1bits.POLH = 0;
        LATAbits.LATA0 = 0;
        LATAbits.LATA1 = 1;
        motor_dx.PID_out = abs(motor_dx.PID_out);
    }
    else if ((motor_dx.PID_out > 0)&&(motor_dx.speed_reqest > 0))//&& (abs(outPID_1) > dead_zone))
    {
        IOCON1bits.POLH = 0;
        LATAbits.LATA0 = 1;
        LATAbits.LATA1 = 0;
    }
    else
    {
        // il motore dovrebbe essere frenato
        IOCON1bits.POLH = 1;
        LATAbits.LATA0 = 0;
        LATAbits.LATA1 = 0;
        motor_dx.PID_out = 0;
        PIDInit(&fooPID_dx); 
    }

    if ((motor_sx.PID_out < 0)&&(motor_sx.speed_reqest < 0))// && (abs(outPID_2) > dead_zone))
    {
        IOCON2bits.POLH = 0;
        LATBbits.LATB0 = 0;
        LATBbits.LATB1 = 1;
        motor_sx.PID_out = abs(motor_sx.PID_out);
    }
    else if ((motor_sx.PID_out > 0)&&(motor_sx.speed_reqest > 0))//&& (abs(outPID_2) > dead_zone))
    {
        IOCON2bits.POLH = 0;
        LATBbits.LATB0 = 1;
        LATBbits.LATB1 = 0;
    }
    else
    {
        // il motore dovrebbe essere frenato
        IOCON2bits.POLH = 1;
        LATBbits.LATB0 = 0;
        LATBbits.LATB1 = 0;
        motor_sx.PID_out = 0;
        PIDInit(&fooPID_dx); 
    }
    
    SetHSPWMDutyCycle1(motor_dx.PID_out);
    SetHSPWMDutyCycle2(motor_sx.PID_out);
}

