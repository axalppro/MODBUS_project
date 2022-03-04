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
    ADCON1Lbits.SSRC = 0b0111;   //Set Sampling to auto mode
    
    
    
    BUFREGEN = 1;          // Use buffer to stock the converted value
    ADCON3H = 0x9F;         //Using RC clock + 31TAD
   // ADCON3L = 0x02;
   // ADCON2L = 0x0;
    
    MODE12 = 1;   //Set AD in 12 bits
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
    double sum = 0;
    switch(channel){
        case 0:
            ADCHS0Lbits.CH0SA = 0; // Use AN0 
            for(int i = 0; i < AVERAGE_SAMPLES; i++)
            {
                SAMP = 1;
                while(DONE != 1){}
                sum += ADCBUF0;
            }
            return (uint16_t)(sum/AVERAGE_SAMPLES);
            break;
        case 1:
            ADCHS0Lbits.CH0SA = 1; // Use AN0 
            for(int i = 0; i < AVERAGE_SAMPLES; i++)
            {
                SAMP = 1;
                while(DONE != 1){}
                sum += ADCBUF1;
            }
            return (uint16_t)(sum/AVERAGE_SAMPLES);
            break;
        default:
            break;
          
    }
    
    
}

uint16_t measure_voltage()
{
    return  ((uint32_t)measure_adc(VOLTAGE_CHANNEL)*ADC_REFH)/ADC_RESOLUTION;
}

uint16_t measure_current(uint16_t offset)
{
    uint32_t temp = (((uint32_t)measure_adc(CURRENT_CHANNEL)*ADC_REFH)/ADC_RESOLUTION);
    temp = (uint32_t)((temp*1000/GAIN)/RESISTOR);
    
    if(offset > temp)
    {
        temp = 0;
    }
    else
    {
        temp = temp -offset;
    }
    return temp;
}