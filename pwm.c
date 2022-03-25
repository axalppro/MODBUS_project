#include "pwm.h"
#include <xc.h>

void pwm_init(void) {  
    
	RPOR10_11bits.RPO11R = 8; // set pin RC2 as CCP4 output  
    
    CCP4CONbits.CCP4M = 0b1100;   //Set PWM mode
    T2CONbits.T2CKPS1 = 0;      //Prescale 4
    T2CONbits.T2CKPS0 = 1;      //Prescale 4
    T2CONbits.TMR2ON = 1;         //Enable TMR2
    
    PR2 = 250;      //Set the PWM periode to 1/15.9kHz
    
    TRISCbits.TRISC2 = 0x00; //Set RC2 as output
}

void pwm_set(uint16_t duty) {
    //The duty cycle can be defined with 10bits

    CCP4CONbits.DC4B = (duty & 0x03); //The two LSB    
    CCPR4L = (duty & 0x3FC)>>2; //The eight MSB
    
}