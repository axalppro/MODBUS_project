#include "pwm.h"
#include <xc.h>

void pwm_init(void) {  
	RPOR10_11bits.RPO11R = 8; // set pin RC2 as CCP4 output  
  // TODO - > complete pwm initialisation code 
}

void pwm_set(uint16_t duty) {
  // TODO - > complete pwm set code 
}