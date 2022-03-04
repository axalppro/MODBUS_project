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

uint8_t tmpStr[30];
uint16_t u; ///< Voltage in milliovolts
uint16_t i; ///< Current in microamperes
uint16_t p; ///< Power in microwatts

uint16_t offset;
uint16_t pwm = 512;
uint8_t msg_val[7] = {0,0,0,0,0,0,0};

void delay_ms(int t)
{
    for(int i = 0; i< t; i++)
    {
        __delay_ms(1);
    }
}

void main(void)
{
	init_hw();
	
	LCD_2x16_WriteMsg((unsigned char *)"Welcome !       ", 0); // display on line 0
	LCD_2x16_WriteMsg((unsigned char *)"*-*-*-*-*-*-*-*-", 1); // display on line 1
	    
    pwm_set(0);
    
    delay_ms(1);
    
    offset = measure_current(0);
    
    
    char msg_char[8] = ": tesffO";
        
    uart_send(msg_char, 8);
    
    uint16_t c_temp = offset;
    msg_val[2] = (uint8_t)(0x30 + c_temp%10);
    c_temp = c_temp/10;
    msg_val[3] = (uint8_t)(0x30 + c_temp%10);
    c_temp = c_temp/10;
    msg_val[4] = (uint8_t)(0x30 + c_temp%10);
    c_temp = c_temp/10;
    msg_val[5] = (uint8_t)(0x30 + c_temp%10);
    c_temp = c_temp/10;
    msg_val[6] = (uint8_t)(0x30 + c_temp%10);
    msg_val[1] = '\n';
    msg_val[0] = '\r';   
    
    uart_send(msg_val, 7);
    
    pwm_set(500);
   

	while (true) {
	// TODO -> complete the main loop
        
        u = measure_current(offset);
              
        
                
        uint16_t u_temp = u;
        msg_val[2] = (uint8_t)(0x30 + u_temp%10);
        u_temp = u_temp/10;
        msg_val[3] = (uint8_t)(0x30 + u_temp%10);
        u_temp = u_temp/10;
        msg_val[4] = (uint8_t)(0x30 + u_temp%10);
        u_temp = u_temp/10;
        msg_val[5] = (uint8_t)(0x30 + u_temp%10);
        u_temp = u_temp/10;
        msg_val[6] = (uint8_t)(0x30 + u_temp%10);
        msg_val[1] = '\n';
        msg_val[0] = '\r';        
        
        
        
        
        uart_send(msg_val, 7);
        
        delay_ms(500);
        
        
        
	}
}