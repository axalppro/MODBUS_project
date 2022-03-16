#include "modbus.h"
#include "crc.h"
#include "uart.h"
#include <xc.h>
#include <stdint.h>
#include <stdio.h>

// Modbus functions
#define READ_INPUT_REGISTERS    0x04
#define READ_HOLDING_REGISTERS  0x03
#define WRITE_SINGLE_REGISTER   0x06

// Modbus data model
uint8_t modbusAddress;
uint16_t input_registers[2];
uint16_t holding_registers[2];


/**
 * Buffers for serial receive and send operations 
 * which are more than one byte long
 **/
uint8_t rx_buf[256];
uint8_t tx_buf[256];

// Current position pointer for storing receive position
uint8_t recPtr = 0;

void modbus_timer(void)
{
    //Turn the timer off until the next char is received
    TMR1ON = 0;
    
    modbus_analyse_and_answer();

    recPtr = 0; //Reset the index
    
    TMR1IF = 0; //Reset the flag
   
}


/* This function analyze the recevied bytes
    it determines if the frame is valid and then 
    execute the function */
uint8_t modbus_analyse_and_answer(void)
{
    //Ignore the frame if the adress is incorrect
    if(rx_buf[0] != modbusAddress)
    {
        return;
    }
    
    //Get the transmitted and calculated CRC to compare
    uint16_t crc_received = ((uint16_t)(rx_buf[recPtr-1])<<8) + rx_buf[recPtr-2];  
    uint16_t crc_calc = CRC16(rx_buf, recPtr-2);  

    //Ignore the frame if the CRC are different
    if(crc_received != crc_calc)
    {
        return;                        
    }

    /*Formating the response frame*/

    tx_buf[0] = rx_buf[0];  //Adress
    tx_buf[1] = rx_buf[1];  //Modbus function code

    //Temp. variables used in the switch case
    uint16_t start_adr;
    uint16_t n_register;
    uint16_t value;
    
    //Depending on the function code
    switch(rx_buf[1])
    {
        case READ_INPUT_REGISTERS:   
            
            //Extract start adress and number of registers to read from the frame
            start_adr = ((uint16_t)(rx_buf[2])<<8) + rx_buf[2];            
            n_register  = ((uint16_t)(rx_buf[4])<<8) + rx_buf[5];
            
            tx_buf[2] = 2* n_register; //Number of registers that will be sent back
            
            for(int i = 0; i < n_register; i++)
            {
                tx_buf[3+(i*2)] = input_registers[start_adr+i]>>8;
                tx_buf[4+(2*i)] = input_registers[start_adr+i];
            }  

            modbus_send(3+2*n_register); //Call modbus_send() with the response frame's size (w/o CRC)
        
            break;
            
        case READ_HOLDING_REGISTERS:   

            //Extract start adress and number of registers to read from the frame
            start_adr = ((uint16_t)(rx_buf[2])<<8) + rx_buf[2];            
            n_register  = ((uint16_t)(rx_buf[4])<<8) + rx_buf[5];
            
            tx_buf[2] = 2* n_register;  //Number of registers that will be sent back
            
            for(int i = 0; i < n_register; i++)
            {
                tx_buf[3+(i*2)] = holding_registers[start_adr+i]>>8;
                tx_buf[4+(2*i)] = holding_registers[start_adr+i];
            }  
            modbus_send(3+2*n_register);    //Call modbus_send() with the response frame's size (w/o CRC)
        
            break;
            
        case WRITE_SINGLE_REGISTER:   
            //Extract adress and value to write in the register
            start_adr = ((uint16_t)(rx_buf[2])<<8) + rx_buf[2];            
            value  = ((uint16_t)(rx_buf[4])<<8) + rx_buf[5];
            
            holding_registers[start_adr] = value;
            
            //Returning the adress and value that has been changed
            tx_buf[2] = start_adr>>8;
            tx_buf[3] = start_adr;
            tx_buf[4] = value>>8;
            tx_buf[5] = value;
                    
            modbus_send(6); //Call modbus_send() with the response frame's size (w/o CRC)
        
            break;
            
        default:
            break;
    }
    
}

void modbus_char_recvd(uint8_t c)
{
	// TODO -> complete modbus char receive 
    RC1IF = 0;
    
    rx_buf[recPtr++] = c;
    
    TMR1ON = 1;
    TMR1H = 0xDC;
    TMR1L = 0xD7;
    
    //uart_send(&c, 1);
}

void modbus_send(uint8_t length)
{
	// TODO -> complete modbus CRC calculation
    uint16_t crc_tx = CRC16(tx_buf, length);
            
    tx_buf[length] = (uint8_t)crc_tx;
    tx_buf[length+1] = (uint8_t)(crc_tx>>8);
    
	length += 2; // add 2 CRC bytes for total size

	// For all the bytes to be transmitted
    uart_send(tx_buf,length);
}

void modbus_init(uint8_t address)
{
	modbusAddress = address;
    
    
    //Configuring timer for modbus usage  
    
    TMR1H = 0xDC;
    TMR1L = 0xD7;
    
    T1CKPS0 = 1;  //PS = 8
    T1CKPS1 = 1;  //PS = 8
    
    T1CONbits.RD16 = 1; //Timer working with 16bit
    
    TMR1ON = 0; //TMR off at start
    TMR1IE = 1; //Interrupt enabled
    
}