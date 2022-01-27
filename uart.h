/************************************************************************//**
 * \file	pwm.h
 * \brief	PWM functions
 ***************************************************************************/

#ifndef UART_H
#define	UART_H

#include <stdint.h>

/**
 * Initializes the UART 
 */
void uart_init(void);

/**
 * Sets the PWM duty
 * @param dataPtr : pointer to the message to send
 * @param length : length of message to send
 */
void uart_send(uint8_t * dataPtr, uint8_t length);

#endif
