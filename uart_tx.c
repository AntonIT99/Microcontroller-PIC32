
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  @Remarks
    Any additional remarks
 */
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
#define temps 15 // pour 1s
#define FCY 11980800
#define BAUDRATE 9600
#define BRG_VALUE ((FCY/BAUDRATE)/16) - 1

//void delayMS2()
//{
//    int counter = 0;
//    int target = temps;
//    //T1CON =0;
//    //T3CON=0;
//    T2CON = 0x0; //stop timer and clear control register : internal peripheral clk
//    T2CONbits.T32 = 0; // 16 bits
//    TMR2 = 0x0; //clear timer2 register (seul les 16 bits de poids faible servent)
//    PR2 = 0xFFFF;
//    IFS0bits.T2IF = 0; // Clear Timer2 Interrupt Flag
//    IEC0bits.T2IE = 0; // Disable Timer2 Interrupt
//    T2CONbits.TCKPS0 = 0;
//    T2CONbits.TCKPS1 = 1; 
//    T2CONbits.TCKPS2 = 0; 
//    T2CONbits.ON = 1; 
//    while (counter < target )
//    {
//        
//        if ( IFS0bits.T2IF == 1)
//        {
//            counter++;
    //            IFS0bits.T2IF = 0;
//        }
//    }
//    counter =0;
//}
void delayMS() // 65 ms ??? => pour remplir 2^16 donc 1us = 1inc ? donc FPB=1 MHz => Sclock => 8MhZ (yes!!)
{
    int counter = 0;
    int target = temps;
    //T1CON =0;
    //T3CON=0;
    T1CON = 0x0; //stop timer and clear control register : internal peripheral clk
    TMR1 = 0x0; //clear timer2 register (seul les 16 bits de poids faible servent)
    PR1 = 0xFFFF;
    IFS0bits.T1IF = 0; // Clear Timer2 Interrupt Flag
    IEC0bits.T1IE = 0; // Disable Timer2 Interrupt
    T1CONbits.TCKPS0 = 0;
    T1CONbits.TCKPS1 = 0; 
    T1CONbits.ON = 1; 
    while (counter < target )
    {
        
        if ( IFS0bits.T1IF == 1)
        {
            counter++;
            IFS0bits.T1IF = 0;
        }
    }
    counter =0;
}



void initUART(void);
void SendChar(char c);
void SendString(char *string);
char ReadChar(void);
void ReadString(char *string, int length);
 
  
void initUART(void)
{
    U2MODEbits.BRGH = 0;                // Baud Rate = 9600
    U2BRG = BRG_VALUE;
    
    U2MODEbits.SIDL = 0;                // Continue operation in SLEEP mode
    
    U2MODEbits.IREN = 1;                // IrDA is disabled
    
    U2MODEbits.RTSMD = 0;               // U1RTS pin is in Flow Control mode
    
    U2MODEbits.UEN = 0b00;              // U1TX, U1RX are enabled
    
    U2MODEbits.WAKE = 1;                // Wake-up enabled
    
    U2MODEbits.LPBACK = 0;              // Loopback mode is disabled
    
    U2MODEbits.RXINV = 0;               // U1RX IDLE state is '1'
    
    U2MODEbits.PDSEL = 0b00;            // 8-bit data, no parity
    
    U2MODEbits.STSEL = 0;               // 1 stop bit
    
    U2STAbits.UTXINV = 0;               // U1TX IDLE state is '1'
    
    U2MODEbits.ON = 1;                  // UART1 is enabled
    
    U2STAbits.URXEN = 1;                // UART1 receiver is enabled
   
    U2STAbits.UTXEN = 1;                // UART1 transmitter is enabled
}
  
 
void SendChar(char c)
{
    U2STAbits.UTXEN = 1;                // Make sure transmitter is enabled
    // while(CTS)                       // Optional CTS use
    while(U2STAbits.UTXBF);             // Wait while buffer is full
    U2TXREG = c;                        // Transmit character
}
  
 
void SendString(char *string)
{
    
   int i = 0;
    
    U2STAbits.UTXEN = 1;                // Make sure transmitter is enabled
    
    while(*string)
    {
        while(U2STAbits.UTXBF);         // Wait while buffer is full
        U2TXREG = *string;              // Transmit one character
        string++;                       // Go to next character in string
    }
}
  
 /*
char ReadChar(void)
{
    //PORTDbits.RD15 = 0;                // Optional RTS use
    while(!U2STAbits.URXDA);             // Wait for information to be received
    //PORTDbits.RD15 = 1;
    return U2RXREG;                      // Return received character
}
  
 
void ReadString(char *string, int length)
{  
    int count = length;
    
    do
    {
        *string = ReadChar();               // Read in character
        //SendChar(*string);                  // Echo character
        
        if(*string == 0x7F && count>length) // Backspace conditional
        {
            length++;
            string--;
            continue;
        }
        
        if(*string == '\r')                 // End reading if enter is pressed
            break;
        
        string++;
        length--;
        
    }while(length>1);
    
    *string = '\0';                         // Add null terminator
}
*/
main()
{
  
   initUART();
   TRISA= 0;
   TRISC= 0;
   LATA = 0x000F;
   // TRISDbits.TRISD14 = 1;   //SW1 as input
    //TRISDbits.TRISD14 = 1;   //SW2 as input
   // PORTCbits.RC2 = 0;                 // Set RTS and CTS pins to 0
   // PORTBbits.RB1 = 0;
    
   char s ='A';
   int pb=0;
    while(1)
    {
    SendChar(s);
    pb=pb+1; 	// Pour faciliter le positionnement d'un pointbreak à cet endroit
     
    }
}


