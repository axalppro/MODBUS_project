/************************************************************************//**
 * \file	main.c
 * \brief	Program main file for MODBUS controller
 ***************************************************************************/


#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "lcd/lcd.h"
#include "pwm.h"
#include "measure.h"
#include "modbus.h"
#include "uart.h"

#define _XTAL_FREQ  64000000L
/**
 * Initialize clock, buttons, leds and interrupts
 */
void init_hw()
{
	OSCCON3 = 0; // runs at 64MHz
	LCD_Init(LCD_2x16);
	adc_init();
	modbus_init(0x80);
	pwm_init();
    uart_init();
	// Interrupts configuration
	PEIE = 1;
	GIE = 1;
}

//Global variables
uint16_t u; ///< Voltage in milliovolts
uint16_t i; ///< Current in microamperes
uint16_t p; ///< Power in microwatts
uint16_t offset; ///< Current offset in microamperes

void delay_ms(int t)
{
    for(int i = 0; i< t; i++)
    {
        __delay_ms(1);
    }
}

void main(void)
{
    //First initialize everything
    init_hw();
	
    //Write a message on the LCD
	LCD_2x16_WriteMsg((unsigned char *)"Modbus !        ", 0); // display on line 0
	LCD_2x16_WriteMsg((unsigned char *)"*-*-*-*-*-*-*-*-", 1); // display on line 1
	    
    /*We need to measure the offset of the current*/

    //PWM to 0 -> 0V on the resitor mesured 
    pwm_set(0);    
    delay_ms(100);    //Need to wait a bit before starting the conversion
    offset = measure_current(0); //Measure the offset
    delay_ms(500);
    /*--------------------------------------------*/
    

	while (true) {
	      
        //Updating the PWM value
        pwm_set(holding_registers[0]);
        
        //Write the current voltage
        input_registers[0] = measure_voltage();
        __delay_us(100);
        
        //Write the current current
        input_registers[1] = measure_current(offset);
        __delay_us(100);
     
        
	}
}