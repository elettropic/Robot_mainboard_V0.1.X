//#include <xc.h>
//#include <stdint.h>             /* For uint16_t definition                      */
//#include <stdbool.h>            /* For true/false definition                    */
//#include <pps.h>                /* For PPS module                               */
//#include <hspwm.h>              /* For PWM module                               */
//#include <InCap.h>              /* For Input capture module                     */
//#include <timer.h>              /* For Timers module                            */
//#include <uart.h>               /* For UART module                              */
//#include <libpic30.h>
#include <dsp.h>
#include <libpic30.h>
#include "struct.h"
#include "uart_control.h"

void ConfigurePeripheral(void);

motor motor_dx;
motor motor_sx;

tPID fooPID_dx;
tPID fooPID_sx;

/* The fooPID data structure contains a pointer to derived coefficients in X-space and */
/* pointer to controler state (history) samples in Y-space. So declare variables for the */
/* derived coefficients and the controller history samples */
fractional abcCoefficient_dx[3] __attribute__ ((section (".xbss, bss, xmemory")));
fractional controlHistory_dx[3] __attribute__ ((section (".ybss, bss, ymemory")));
fractional abcCoefficient_sx[3] __attribute__ ((section (".xbss, bss, xmemory")));
fractional controlHistory_sx[3] __attribute__ ((section (".ybss, bss, ymemory")));
/* The abcCoefficients referenced by the fooPID data structure */
/* are derived from the gain coefficients, Kp, Ki and Kd */
/* So, declare Kp, Ki and Kd in an array */
fractional kCoeffs_dx[] = {0,0,0};
fractional kCoeffs_sx[] = {0,0,0};

double kp = 0.50;
double ki = 0.24;
double kd = 0.005;

unsigned char consoleBuffer[CONSOLE_BUF_LEN];
char * Receiveddata = consoleBuffer;
int buffWrite;
unsigned char siFrameReady;
unsigned char command_type;