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
	
	// TODO - > complete uart initialisation code 
}

void uart_send(uint8_t * dataPtr, uint8_t length)
{
  // TODO - > complete uart sending code 
}