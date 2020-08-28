// DEVCFG3
#pragma config USERID = 0xFFFF          // Enter Hexadecimal value (Enter Hexadecimal value)
#pragma config PMDL1WAY = ON            // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow only one reconfiguration)
#pragma config FUSBIDIO = ON            // USB USID Selection (Controlled by the USB Module)
#pragma config FVBUSONIO = ON           // USB VBUS ON Selection (Controlled by USB Module)

// DEVCFG2
#pragma config FPLLIDIV = DIV_12        // PLL Input Divider (12x Divider)
#pragma config FPLLMUL = MUL_24         // PLL Multiplier (24x Multiplier)
#pragma config UPLLIDIV = DIV_12        // USB PLL Input Divider (12x Divider)
#pragma config UPLLEN = OFF             // USB PLL Enable (Disabled and Bypassed)
#pragma config FPLLODIV = DIV_256       // System PLL Output Clock Divider (PLL Divide by 256)

// DEVCFG1
#pragma config FNOSC = FRCDIV           // Oscillator Selection Bits (Fast RC Osc w/Div-by-N (FRCDIV))
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_8           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/8)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1              // Watchdog Timer Postscaler (1:1)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config JTAGEN = ON              // JTAG Enable (JTAG Port Enabled)
#pragma config ICESEL = ICS_PGx4        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

#include <xc.h>

void ADC_init (void);
int ReadADC(void);

void ADC_init ()
{

AD1CON1bits.SSRC = 0b111; //Internal counter ends sampling and starts conversion (auto convert)    
AD1CON1bits.FORM = 0; //Unsigned Integer 16-bit
AD1CON2 = 0;  // on utilise les tensions de références du CAN
AD1CHS =0x50000;  // on utilise l?entrée AN1 en analogique
AD1CSSL=0; //pas de scan d?entrées
 
 
AD1CON3bits.ADRC = 0; //Clock derived from Peripheral Bus Clock (PBCLK)  source de l?horloge = horloge des périphériques     
AD1CON3bits.ADCS = 0b00111111; // 128*TPB = TAD (prediviseur= 64)
     
AD1CON3bits.SAMC = 0b11111; //31*TAD (temps d?autoéchantillonnage)
AD1CON1bits.ON = 1; //on active le CAN            
 
}
int ReadADC()
{
    int test = 0;
    AD1CON1bits.SAMP = 1; //activation échantillonnage
    while(!AD1CON1bits.DONE) //tant que la conversion n'est pas finie
    {
    }

return ADC1BUF0; //on renvoie le résultat
}

void main ()
{
   int ValeurADC;
   TRISB = 0;
   ANSELB = 0;
   
   TRISBbits.TRISB3 = 1; //configuration input du pin 24 (BUTTON_ADC)
   TRISA= 0; //sorties output
   TRISC= 0; //sorties output
   LATA = 0x000F; //LEDs rouge et verte allumées
   ADC_init();
 
    while(1)
    {   
        ValeurADC=ReadADC();
        if (ValeurADC < 0x220) //si le bouton S1 est appuyé
        {
           LATAbits.LATA0 = 0; //LED rouge éteinte
           LATAbits.LATA1 = 1; //LED verte allumée
           LATCbits.LATC9 = 1; //LED bleue allumée
        }
    }
}