/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file, thus actually include <p33FJ128MC804.h> */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC33E__)
    	#include <p33Exxxx.h>
    #elif defined(__dsPIC33F__)
    	#include <p33Fxxxx.h>
    #endif
#endif

#if defined(__XC16__)
    //#include <p33FJ128MC804.h> 
    #include "system.h"        /* System funct/params, like osc/peripheral config */

    /* TODO DSPIC33FJ128MC804 Configuration Bit Settings*/
    // 'C' source line config statements
    // FBS
    #pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
    #pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)
    #pragma config RBS = NO_RAM             // Boot Segment RAM Protection (No Boot RAM)

    // FSS
    #pragma config SWRP = WRPROTECT_OFF     // Secure Segment Program Write Protect (Secure segment may be written)
    #pragma config SSS = NO_FLASH           // Secure Segment Program Flash Code Protection (No Secure Segment)
    #pragma config RSS = NO_RAM             // Secure Segment Data RAM Protection (No Secure RAM)

    // FGS
    #pragma config GWRP = OFF               // General Code Segment Write Protect (User program memory is not write-protected)
    #pragma config GSS = OFF                // General Segment Code Protection (User program memory is not code-protected)

    /*
    _FOSCSEL(FNOSC_FRC);                                  // Select Internal FRC at POR
    _FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);       // Enable Clock Switching and Configure Posc in XT mode
    _FPOR(RST_PWMPIN & PWM1H_ACT_HI & PWM1L_ACT_HI);      // High and Low switches set to active-high state 
    */

    // FOSCSEL
    #pragma config FNOSC = PRI              // Primary Oscillator (XT, HS, EV))
    #pragma config IESO = ON                // Internal External Switch Over Mode (Start-up device with FRC, then automatically switch to user-selected oscillator source when ready)

    // FOSC
    #pragma config POSCMD = XT              // XT Oscillator Mode
    #pragma config OSCIOFNC = OFF           // OSC2 Pin Function (OSC2 pin has clock out function)
    #pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow Only One Re-configuration)
    #pragma config FCKSM = CSECMD           // This bit is extremely important? if set to CSDCMD there will be no PWM signal!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //#pragma config FCKSM = CSDCMD         // Clock Switching and Monitor (Both Clock Switching and Fail-Safe Clock Monitor are disabled)

    // FWDT
    #pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler (1:32,768)
    #pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
    #pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
    #pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog timer always enabled)

    // FPOR
    #pragma config FPWRT = PWR128           // POR Timer Value (128ms)
    #pragma config ALTI2C = OFF             // Alternate I2C  pins (I2C mapped to SDA1/SCL1 pins)
    #pragma config LPOL = ON                // Motor Control PWM Low Side Polarity bit (PWM module low side output pins have active-high output polarity)
    #pragma config HPOL = ON                // Motor Control PWM High Side Polarity bit (PWM module high side output pins have active-high output polarity)
    #pragma config PWMPIN = OFF             // Motor Control PWM Module Pin Mode bit (PWM module pins controlled by PORT register at device Reset)

    // FICD
    #pragma config ICS = PGD3               // Comm Channel Select (Communicate on PGC3/EMUC3 and PGD3/EMUD3)
    #pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is Disabled)
#endif

#include "user.h"          /* User funct/params, such as InitApp              */
#define PI 3.1415926

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/
/* i.e. uint16_t <variable_name>; */
char ReceivedChar;
char TransmitChar;
bool go = 0;
bool stop = 0;
bool direction[3] = {0,0,0};

int count[6] = {0,0,0,0,0,0};
int motor[3] = {0,0,0};
int i = 0;

int counter = 0;
int countMatrix = 0;
int countVector = 0;
int countMatrixGlobal = 0;
int countVectorGlobal = 0;
float triMatrix[3][3] = { 6,0,0,0,6,0,0,0,6 };
clock_t initTime;
clock_t realTime;

Matrix* pointerMatrix[100];
Vector3f* pointerVector[100];
Matrix* pointerMatrixGlobal[100];
Vector3f* pointerVectorGlobal[100];
Parameter P;
Matrix *Kp;
Matrix *Kd;
/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
int main(void)
{   
    /* Configure the oscillator for the device */
    ConfigureOscillator();
    /* Initialize IO ports and peripherals */
    InitApp();
    /* TODO <INSERT USER APPLICATION CODE HERE> */
    Kp = m_constructor(global, NULL, NULL, 6, 0, 0, 0, 6, 0, 0, 0, 6);
	Kd = m_constructor(global, NULL, NULL, 10, 0, 0, 0, 10, 0, 0, 0, 10);
    Vector3f* qd = v_constructor(global, NULL, 0, 0, 0);
	Vector3f* dqd = v_constructor(global, NULL, 0, 0, 0);
	Vector3f* ddqd = v_constructor(global, NULL, 0, 0, 0);
    Vector3f* q = v_constructor(global, NULL, 0, 0, 0);
	Vector3f* dq = v_constructor(global, NULL, 0, 0, 0);
    Vector3f* controlEffect;
	Vector3f* ddq;
    P.m = 11.4;
	P.Iv = 0.65;
	P.r = 0.05;
	P.Din = 0.147;
	P.Dout = 0.236;
	//P.La = (P.Din + P.Dout) / 2;
	P.La = 0.2425;
	P.I0 = 6 * pow(10, -6);
	P.kt = 0.0208;
	//P.kb = 1 / 34.34;
	P.kb = 0.02076;
	P.n = 71;
	P.b0 = 6.0 * pow(10, -5);
	P.Ra = 1.53;
	P.beta0 = pow(P.n, 2) * P.I0 / pow(P.r, 2);
	P.beta1 = pow(P.n, 2) * (P.b0 + P.kt*P.kb / P.Ra) / pow(P.r, 2);
	P.beta2 = P.n*P.kt / P.r / P.Ra;
    initTime = clock();
    while(1)
    {
        if(U1STAbits.PERR==1)
        {
            continue;
        }
        if(U1STAbits.OERR==1)
        {
            //LATAbits.LATA0=1;
            U1STAbits.OERR=0;
            //receivedNumber++;
            continue;       //continue statement must be in a loop;
        }
        if(U1STAbits.URXDA==1)
        {   
            //LATAbits.LATA1=1;
        }
        realTime = clock() - initTime;
        qd->x = cos(realTime*PI / 15);
		qd->y = sin(realTime*PI / 15);
		qd->z = 0;
        dqd->x = -PI/15*sin(realTime*PI / 15);
		dqd->y = PI/15*cos(realTime*PI / 15);
		dqd->z = 0;
        ddqd->x = -pow(PI/15,2)*cos(realTime*PI / 15);
		ddqd->y = -pow(PI/15,2)*sin(realTime*PI / 15);
		ddqd->z = 0;
        
        controlEffect = OMRS_controller(qd, dqd, ddqd, q, dq);
		ddq = OMRS_model(controlEffect, q, dq);
        
        motor_drive();
        /* release dynamically allocated local memory */
        for (counter = 0; counter < countMatrix; counter++)
		{
			m_destructor(pointerMatrix[counter], 1);
		};
		for (counter = 0; counter < countVector; counter++)
		{
			v_destructor(pointerVector[counter], 1);
		}
		countMatrix = 0;
		countVector = 0;
    };
    /* release dynamically allocated global memory */
    for (counter = 0; counter < countMatrixGlobal; counter++)
	{
		m_destructor(pointerMatrixGlobal[counter], 1);
	};
	for (counter = 0; counter < countVectorGlobal; counter++)
	{
		v_destructor(pointerVectorGlobal[counter], 1);
	}
	countMatrixGlobal = 0;
	countVectorGlobal = 0;
    
    return 0;
}

#if defined(__XC16__)

void motor_drive(void)
{       
    if(stop){
    LATAbits.LATA0=1;
    LATAbits.LATA4=1;
    LATAbits.LATA8=1;
    }
    if(go){
    LATAbits.LATA0=0;
    LATAbits.LATA4=0;
    LATAbits.LATA8=0;
    }

    if(i==0)
    {            
        motor[0] = count[0];
        motor[0] = motor[0] & 0x00FF;
        motor[0] = motor[0] | (count[1]<<8);

        motor[1] = count[2];
        motor[1] = motor[1] & 0x00FF;
        motor[1] = motor[1] | (count[3]<<8);

        motor[2] = count[4];
        motor[2] = motor[2] & 0x00FF;
        motor[2] = motor[2] | (count[5]<<8);
    }
    int j=0;
    for(j=0;j<3;j++)
    {
        int temp=0;
        temp = motor[j] & 0x8000;
        if(temp)
        {
            motor[j]=motor[j] & 0x7fff ;
            //motor[j]=~(motor[j]-1);
            direction[j]=0;
        }else{direction[j]=1;}
    }
    LATAbits.LATA1=direction[0];
    LATAbits.LATA7=direction[1];
    LATAbits.LATA9=direction[2];
    P1DC1=(5*motor[0]/3);
    P1DC2=(5*motor[1]/3);
    P1DC3=(5*motor[2]/3);
}

void __attribute__((__interrupt__,auto_psv)) _U1RXInterrupt(void)
{
    ReceivedChar = U1RXREG;
    
    if(ReceivedChar == 'g'){go = 1;}
    else if(ReceivedChar == 's'){stop = 1; go = 0;}
    else 
    {
        
    if(ReceivedChar == 'u'){ U1TXREG = 'u'; i = 0;}
    else
    {
        count[i] = ReceivedChar;
        i++;
        if(i>=6) i = 0;
    }
    
    }
    
    IFS0bits.U1RXIF = 0;
}

void __attribute__((__interrupt__,auto_psv)) _U1TXInterrupt(void)
{
    IFS0bits.U1TXIF = 0; // clear TX interrupt flag
    //U1TXREG = 'b'; // Transmit one character
   
}

#endif