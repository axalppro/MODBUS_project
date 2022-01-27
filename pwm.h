/************************************************************************//**
 * \file	pwm.h
 * \brief	PWM functions
 ***************************************************************************/

#ifndef PWM_H
#define	PWM_H

#include <stdint.h>

/**
 * Initializes the PWM pin and timer
 */
void pwm_init(void);

/**
 * Sets the PWM duty
 * @param duty : the duty cycle (0..1023)
 */
void pwm_set(uint16_t duty);

#endif
