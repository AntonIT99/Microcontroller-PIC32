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

#define SLAVE_ADDR 0x18

#include <xc.h>

void i2c_master_setup(void) ;
void i2c_master_start(void) ;
void i2c_master_restart(void);
void i2c_master_send(unsigned char byte);
unsigned char i2c_master_recv(void);
void i2c_master_stop(void);
void i2c_master_ack(int val);


void i2c_master_setup(void)
{
double Fsck = 100000;
double PGD = 0.000000104;
double Pblck = 1000000;
I2C1BRG = (int)((1/(2*Fsck) - PGD)*Pblck - 2); // I2CBRG = [1/(2*Fsck) - PGD]*Pblck - 2
// Fsck is the freq (100 kHz here), PGD = 104 ns
I2C1CONbits.ON = 1; // turn on the I2C1 module
}

// Start a transmission on the I2C bus // fct start
void i2c_master_start(void)
{
I2C1CONbits.SEN = 1; // send the start bit
while(I2C1CONbits.SEN) { ; } // wait for the start bit to be sent
}

void i2c_master_restart(void) // fct restart
{
I2C1CONbits.RSEN = 1; // send a restart
while(I2C1CONbits.RSEN) { ; } // wait for the restart to clear
}

void i2c_master_send(unsigned char byte)
{ // send a byte to slave
int a=0;
I2C1TRN = byte; // if an address, bit0=0forwrite, 1 for read
while(I2C1STATbits.TRSTAT)
{ ;
} // wait for the transmission to finish
if(I2C1STATbits.ACKSTAT)
{ // if this is high, slave has not acknowledged
a=1;
}
}

unsigned char i2c_master_recv(void)  
{ // receive a byte from the slave
I2C1CONbits.RCEN = 1; // start receiving data
while(!I2C1STATbits.RBF) { ; } // wait to receive the data
return I2C1RCV; // read and return the data
}


void i2c_master_ack(int val)
{ // sends ACK = 0 (slave should send another byte)
// or NACK = 1 (no more bytes requested from slave)
I2C1CONbits.ACKDT = val; // store ACK/NACK in ACKDT
I2C1CONbits.ACKEN = 1; // send ACKDT
while(I2C1CONbits.ACKEN) { ; } // wait for ACK/NACK to be sent
}

void i2c_master_stop(void) // fct stop ok
{ // send a STOP:
I2C1CONbits.PEN = 1; // comm is complete and master relinquishes bus
while(I2C1CONbits.PEN) { ; } // wait for STOP to complete
}


void main ()
{

unsigned char master_write0 = 0x00; // first byte that master writes
unsigned char master_read0 = 0x00; // first received byte


i2c_master_setup();
    while(1)
    {  
        i2c_master_start();
        i2c_master_send(SLAVE_ADDR << 1); // send the slave address, left shifted by 1,
        
        i2c_master_send(master_write0); // send a byte to the slave
        
        i2c_master_restart();
        i2c_master_send((SLAVE_ADDR << 1) | 1); // send slave address, left shifted by 1
        
        master_read0 = i2c_master_recv(); // receive another byte from the bus
      
        i2c_master_ack(1); // send NACK (1): master needs no more bytes
        i2c_master_stop(); // send STOP: end transmission, give up bus
        

  }

 
} 