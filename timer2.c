
#include <proc/p32mx270f256d.h>


// Configuration Bits
//#pragma config FSRSSEL = PRIORITY_7 // Shadow Register Set Priority 7
#pragma config PMDL1WAY = ON // Peripheral Module - One Reconfig
#pragma config IOL1WAY = ON // Peripheral Pin Select - One Reconfig
#pragma config FUSBIDIO = ON // USB USID Selection - Port Function
#pragma config FVBUSONIO = ON // USB VBUS ON Selection - Port Function
// DEVCFG2
#pragma config FPLLIDIV = DIV_12 // PLL Input Divider - 12x
#pragma config FPLLMUL = MUL_24 // PLL Multiplier - 24x
#pragma config UPLLIDIV = DIV_12 // USB PLL Input Divider - 12x
#pragma config UPLLEN = OFF // USB PLL Disabled and Bypassed
#pragma config FPLLODIV = DIV_256 // Sys PLL Output Divide by 256
// DEVCFG1
#pragma config FNOSC = FRCDIV // Oscillator - Fast RC Osc w/Div-by-N N=0 par def
#pragma config FSOSCEN = OFF // Secondary Oscillator Disabled
#pragma config IESO = OFF // Internal/External Switch Over Disabled
#pragma config POSCMOD = OFF // Primary Oscillator Disabled
#pragma config OSCIOFNC = OFF // CLKO on OSCO Pin Disabled
#pragma config FPBDIV = DIV_8 // Peripheral Clock Divisor: Sys_Clk/8
#pragma config FCKSM = CSDCMD // Clock Switch Disable, FSCM Disabled
#pragma config WDTPS = PS1048576 // WDT Postscaler 1:1048576
#pragma config WINDIS = OFF // Watchdog Timer is in Non-Window Mode
#pragma config FWDTEN = OFF // WDT Disabled (SWDTEN Control)
#pragma config FWDTWINSZ = WINSZ_25 // Watchdog Timer Window 25%
// DEVCFG0
#pragma config DEBUG = OFF // Background Debugger Disabled
#pragma config JTAGEN = OFF // JTAG Disabled
#pragma config ICESEL = ICS_PGx4 // ICE/ICD Comm Channel PGEC2/PGED2
#pragma config PWP = OFF // Program Flash Write Protect Disabled
#pragma config BWP = OFF // Boot Flash Write Protect Disabled
#pragma config CP = OFF // Code Protect Disabled
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include <xc.h>
#define temps 20 


void delayMS() 
{
    int counter = 0;
    int target = temps;
    //reinitialisation du Timer
    T1CON = 0x0; 
    TMR1 = 0x0;
    
    //configuration du seuil pour avoir 50ms
    PR1 = 0xC350;
    
    IFS0bits.T1IF = 0; 
    IEC0bits.T1IE = 0;
    
    //1:1 prescale value
    T1CONbits.TCKPS0 = 0;
    T1CONbits.TCKPS1 = 0; 
    
    //Timer is enabled
    T1CONbits.ON = 1;
    
    while (counter < target)
    {
        
        if (IFS0bits.T1IF == 1)
        {
            counter++;
            IFS0bits.T1IF = 0;
        }
    }
    counter =0;
}

void temporisation_multiple(int nb_secondes)
{
    int counter = 0;
    int target = temps;
    //reinitialisation du Timer
    T1CON = 0x0; 
    TMR1 = 0x0;
    
    //configuration du seuil pour avoir 50ms
    PR1 = 0xC350;
    
    IFS0bits.T1IF = 0; 
    IEC0bits.T1IE = 0;
    
    //1:1 prescale value
    T1CONbits.TCKPS0 = 0;
    T1CONbits.TCKPS1 = 0; 
    
    //Timer is enabled
    T1CONbits.ON = 1;
    
    while (counter < nb_secondes*target)
    {
        
        if (IFS0bits.T1IF == 1)
        {
            counter++;
            IFS0bits.T1IF = 0;
        }
    }
    counter =0;
}

int main(void)
{
    TRISAbits.TRISA1 = 0; //LED en sortie (output))
   	while(1)
   	{   
        LATAbits.LATA0 = 1;
        temporisation_multiple(1);
        LATAbits.LATA0 = 0;
        temporisation_multiple(1);
    }
    return 0;
}  
    






