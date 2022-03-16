/*
 * File:   uart.c
 * Author: pascal.sartoret
 *
 * Created on 27. janvier 2022, 15:22
 */


#include <xc.h>
#include <stdint.h>
void uart_init(void)
{
	RPOR18_19bits.RPO18R = 1; // map RC6 to RX FTDI (output of CPU) UART1_TX
	RPINR0_1bits.U1RXR = 4; // map RC7 to TX FTDI (input of CPU) UART2_RX
	
    TXSTA1bits.TXEN = 1;  //Enable transmit
    RCSTA1bits.SPEN = 1;  //Enable serial port
    RCSTA1bits.CREN = 1;  //Enable receiving
    
    
    
    BAUDCON1bits.BRG16 = 1;   //16bit baudrate
    TXSTA1bits.BRGH = 0;      //Low speed
    
    RC1IE = 1;
    
    SPBRGH1 = 0x1;    
    SPBRG1 = 0xA0;   //Baudrate 9600(9592)
    
}
void uart_send(uint8_t * dataPtr, uint8_t length)
{
    for(int i = 0; i < length; i++)
    {
        while(TX1IF != 1)
        {}
        TXREG1 = dataPtr[i];
    }
}