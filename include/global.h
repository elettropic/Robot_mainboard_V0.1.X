#include <stdio.h>
#include <xc.h>
#include <stdint.h>             /* For uint16_t definition                      */
#include <stdbool.h>            /* For true/false definition                    */
#include <pps.h>                /* For PPS module                               */
#include <hspwm.h>              /* For PWM module                               */
#include <InCap.h>              /* For Input capture module                     */
#include <timer.h>              /* For Timers module                            */
#include <uart.h>               /* For UART module                              */
#include <libpic30.h>
#include <dsp.h>
#include <math.h>
#include "struct.h"
#include "uart_control.h"

#define MAX_SPEED 180


void PID_setup(void);
void CalculateSpeed (motor*);
void ComputePID(tPID*, motor*);
void SetMotorParameter(void);

void serial_process(void);

extern motor motor_dx;
extern motor motor_sx;

extern double kp;
extern double ki;
extern double kd;

extern tPID fooPID_dx;
extern tPID fooPID_sx;
extern fractional abcCoefficient_dx[3] __attribute__ ((section (".xbss, bss, xmemory")));
extern fractional controlHistory_dx[3] __attribute__ ((section (".ybss, bss, ymemory")));
extern fractional abcCoefficient_sx[3] __attribute__ ((section (".xbss, bss, xmemory")));
extern fractional controlHistory_sx[3] __attribute__ ((section (".ybss, bss, ymemory")));
extern fractional kCoeffs_dx[];
extern fractional kCoeffs_sx[];

extern unsigned char consoleBuffer[CONSOLE_BUF_LEN];
extern char * Receiveddata;
extern int buffWrite;
extern unsigned char siFrameReady;
extern unsigned char command_type;