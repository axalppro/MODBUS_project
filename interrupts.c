#include "modbus.h"
#include <xc.h>

void __interrupt(high_priority) AnyISR(void)
{
	// Byte received
	if ((RC1IE == 1)&&(RC1IF == 1)) {
		modbus_char_recvd(RCREG1);
	}

	// End of frame
	if ((TMR1IE == 1)&&(TMR1IF == 1)) {
		modbus_timer();
	}
}