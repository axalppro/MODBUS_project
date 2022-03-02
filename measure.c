#include <xc.h>

#include "measure.h"

#define VOLTAGE_CHANNEL 0
#define CURRENT_CHANNEL 1

#define ADC_RESOLUTION  4096 - 1
#define ADC_REFH        3300
#define GAIN            66
#define RESISTOR        3

// Number of samples to do the averaging during measures
#define AVERAGE_SAMPLES 8

void adc_init(void)
{
	// TODO -> complete adc initialisation
    
    
    ANSEL0 = 1; //Set RA0 as analog input
    ANSEL1 = 1; //Set RA1 as analog input
    ADCON1Lbits.SSRC = 0b0111;
    
    
    ADCHS0Lbits.CH0SA = 0; // Use AN0 
    ADCON3H = 0x1F;
    ADCON3L = 0x02;
    ADCON2L = 0x0;
    
    MODE12 = 1;
    ADON = 1;   //Turn ON the AD converter
    

}

/**
 * Read one ADC channel. This function is only
 * local to this file.
 *
 * @param channel : the channel to be measured
 * @return the ADC read value
 */
static uint16_t measure_adc(uint8_t channel)
{
	// TODO -> complete adc measure
}

uint16_t measure_voltage()
{
	// TODO -> complete measure of voltage

}

uint16_t measure_current(uint16_t offset)
{
	// TODO -> complete measure of current
}