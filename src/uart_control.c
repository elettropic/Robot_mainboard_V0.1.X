#include "global.h"

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void getJoystickState(unsigned char data[])    
{
    float speed_scale_dx = 0;
    float speed_scale_sx = 0;
    float max_scale = 0;
    int joyX = (data[1]-48)*100 + (data[2]-48)*10 + (data[3]-48);       // obtain the Int from the ASCII representation
    int joyY = (data[4]-48)*100 + (data[5]-48)*10 + (data[6]-48);
    joyX = joyX - 200;                                                  // Offset to avoid
    joyY = joyY - 200;
    if(joyX<-100 || joyX>100 || joyY<-100 || joyY>100)     
        return;      // commmunication error
    joyX = (int)map(joyX,-100,100,-MAX_SPEED,MAX_SPEED);
    joyY = (int)map(joyY,-100,100,-MAX_SPEED,MAX_SPEED);
    motor_dx.speed_reqest = joyY+joyX;
    motor_sx.speed_reqest = joyY-joyX;
    
    speed_scale_dx = motor_dx.speed_reqest/MAX_SPEED;
    speed_scale_dx = abs(speed_scale_dx); 
    speed_scale_sx = motor_sx.speed_reqest/MAX_SPEED;
    speed_scale_sx = abs(speed_scale_sx);
    
    if (speed_scale_dx > speed_scale_sx)
        max_scale = speed_scale_dx;
    else
        max_scale = speed_scale_sx;   
    if (max_scale < 1)
        max_scale = 1;

    motor_dx.speed_reqest = (int)(motor_dx.speed_reqest/max_scale);
    motor_sx.speed_reqest = (int)(motor_sx.speed_reqest/max_scale);
    

    if (motor_dx.speed_reqest > MAX_SPEED)
        motor_dx.speed_reqest = MAX_SPEED;
    else if (motor_dx.speed_reqest < -MAX_SPEED)
        motor_dx.speed_reqest = -MAX_SPEED;
    
    if (motor_sx.speed_reqest > MAX_SPEED)
        motor_sx.speed_reqest = MAX_SPEED;
    else if (motor_sx.speed_reqest < -MAX_SPEED)
        motor_sx.speed_reqest = -MAX_SPEED;
}

void serial_process(void)
{
	if (siFrameReady)
    {        
        if (command_type == 1)
            getJoystickState(consoleBuffer);
        siFrameReady=0;
    
        putcUART1(0x02);
        printf("11111");
        putcUART1(0x01);
        printf("%.2f",motor_dx.speed_actual);
        putcUART1(0x04);
        printf("%.2f",motor_sx.speed_actual);
        putcUART1(0x05);
        printf("Motor OK");
        putcUART1(0x03);

    }
    

    //printf("PID %.2f %.2f %.2f %.2f %.2f %.2f Manual Reverse\n\r",speed_req,speed_act,PWM_val_1,kp,ki,kd);

}