
#include <xc.h>
#include "global.h"

void ConfigureOscillator()
{
    //Configure OSC module to execute instruction at 60MHz (System frequency is 120 MHz)
    // Disable Watch Dog Timer
    RCONbits.SWDTEN = 0;    
    // Configure PLL prescaler, PLL postscaler, PLL divisor
    PLLFBD=85;              // M=87
    CLKDIVbits.PLLPOST=0;   // N2=2
    CLKDIVbits.PLLPRE=2;    // N1=4
    // Initiate Clock Switch to FRC oscillator with PLL (NOSC=0b001)
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(OSCCON | 0x01);
    // Wait for Clock switch to occur
    while (OSCCONbits.COSC!= 0b001);
    // Wait for PLL to lock
    while (OSCCONbits.LOCK!= 1);
}

void ConfigurePORT()
{
    ANSELA = 0x0;               /* Set all port in digital mode */
    ANSELB = 0x0;
    
    PORTBbits.RB11 = 0;         //ingresso Encoder 1
    PORTBbits.RB10 = 0;         //ingresso Encoder 2
    PORTAbits.RA0 = 0;          //controllo direzione 1 motore 1
    PORTAbits.RA1 = 0;          //controllo direzione 2 motore 1
    PORTBbits.RB0 = 0;          //controllo direzione 1 motore 2
    PORTBbits.RB1 = 0;          //controllo direzione 2 motore 2   
    
    TRISAbits.TRISA0 = 0;       //controllo direzione 1 motore 1
    TRISAbits.TRISA1 = 0;       //controllo direzione 2 motore 1
    TRISBbits.TRISB0 = 0;       //controllo direzione 1 motore 2
    TRISBbits.TRISB1 = 0;       //controllo direzione 2 motore 2    
    
    TRISBbits.TRISB11 = 1;      //ingresso Encoder 1
    TRISBbits.TRISB10 = 1;      //ingresso Encoder 2    
    
    TRISAbits.TRISA4 = 1;       //UART RX
    TRISBbits.TRISB4 = 0;       //UART TX
    
    
    PPSInput(IN_FN_PPS_U1RX,IN_PIN_PPS_RP20);       /* Set UART 1 RX to pin RP20 */
    PPSOutput(OUT_FN_PPS_U1TX,OUT_PIN_PPS_RP36);    /* Set UART 1 TX to pin RP36 */
    PPSInput(IN_FN_PPS_IC1,IN_PIN_PPS_RP43);        /* Set IC1 to pin RP43  */
    PPSInput(IN_FN_PPS_IC2,IN_PIN_PPS_RP42);        /* Set IC2 to pin RP42  */
}


void ConfigurePWM()
{  
    CloseHSPWM();
    OpenHSPWM(  PWM_MOD_DIS & PWM_IDLE_CONT ,
                PWM_INPUT_CLK_DIV6,
                2500,0);
    
    /* Set Phase Shift */
    PHASE1 = 0;
    PHASE2 = 0;
    ConfigHSPWMFault1(PWM_IND_FLT_DIS);
    ConfigHSPWMFault2(PWM_IND_FLT_DIS);
    
    ConfigHSPWM1(PWM_FLT_INT_DIS | PWM_TB_MODE_PER | PWM_D_CYLE_DC | PWM_DT_DIS | PWM_CENTER_ALIGN_DIS | PWM_DC_UPDATE_IMM ,
                 PWM_H_PIN_ACTHIGH | PWM_H_PIN_EN | PWM_L_PIN_GPIO,0,0,0);
    ConfigHSPWM2(PWM_FLT_INT_DIS | PWM_TB_MODE_PER | PWM_D_CYLE_DC | PWM_DT_DIS | PWM_CENTER_ALIGN_DIS | PWM_DC_UPDATE_IMM ,
                 PWM_H_PIN_ACTHIGH | PWM_H_PIN_EN | PWM_L_PIN_GPIO,0,0,0);
    
    ConfigHSPWM3(0,PWM_H_PIN_GPIO | PWM_L_PIN_GPIO,0,0,0);
    /* Set Duty Cycles */
    SetHSPWMDutyCycle1(0);
    SetHSPWMDutyCycle2(0);

    /* Set Dead Time Values */
    SetHSPWMDeadTime1(00,00);
    SetHSPWMDeadTime2(00,00);
     
    /* Enable PWM Module */
    PTCONbits.PTEN = 1;			   			/* Enable the PWM Module */
}

void ConfigureInputCapture()
{
    ConfigIntCapture1(IC_INT_ON & IC_INT_PRIOR_6);
    ConfigIntCapture2(IC_INT_ON & IC_INT_PRIOR_6);
    OpenCapture1(IC_IDLE_STOP & IC_TIMER1_SRC & IC_INT_2CAPTURE & IC_EVERY_RISE_EDGE,IC_CASCADE_DISABLE & IC_SYNC_TRIG_IN_DISABLE);
    OpenCapture2(IC_IDLE_STOP & IC_TIMER1_SRC & IC_INT_2CAPTURE & IC_EVERY_RISE_EDGE,IC_CASCADE_DISABLE & IC_SYNC_TRIG_IN_DISABLE);
}

void ConfigureTimer1()
{
    ConfigIntTimer1(T1_INT_PRIOR_7 & T1_INT_ON);
    OpenTimer1(T1_OFF & T1_IDLE_CON & T1_GATE_OFF & T1_PS_1_64 & T1_SOURCE_INT & T1_SYNC_EXT_OFF, 34228);
    T1CONbits.TON = 1;
    EnableIntT1;
}


void ConfigureUART()
{
    /* Holds the value of baud register   */
    unsigned int baudvalue;   
    /* Holds the value of uart config reg */
    unsigned int U1MODEvalue;
    /* Holds the information regarding uart
    TX & RX interrupt modes */   
    unsigned int U1STAvalue; 
    /* Turn off UART1module */
    CloseUART1();
    /* Configure uart1 receive and transmit interrupt */
    ConfigIntUART1(UART_RX_INT_EN & UART_RX_INT_PR5 & 
                  UART_TX_INT_DIS & UART_TX_INT_PR4);
    /* Configure UART1 module to transmit 8 bit data with one stopbit. Also Enable loopback mode  */
    baudvalue = 173;
    U1MODEvalue = UART_EN & UART_IDLE_CON & UART_IrDA_DISABLE &
                  UART_UEN_00 & UART_DIS_WAKE & UART_DIS_LOOPBACK  &
                  UART_MODE_SIMPLEX & UART_DIS_ABAUD & UART_NO_PAR_8BIT  &
                  UART_UXRX_IDLE_ZERO & UART_BRGH_FOUR & UART_1STOPBIT;
    U1STAvalue  = UART_INT_TX_BUF_EMPTY  &  
                  UART_TX_ENABLE & UART_INT_RX_CHAR &
                  UART_ADR_DETECT_DIS &
                  UART_SYNC_BREAK_DISABLED &
                  UART_IrDA_POL_INV_ZERO &
                  UART_RX_OVERRUN_CLEAR;
    OpenUART1(U1MODEvalue, U1STAvalue, baudvalue);
}



void ConfigurePeripheral()
{
    ConfigureOscillator();
    ConfigurePORT();
    ConfigureTimer1();
    ConfigureUART();
    ConfigurePWM();
    ConfigureInputCapture();
}