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

/*Initialize the AD converter*/
void adc_init(void)
{
    ANSEL0 = 1; //Set RA0 as analog input
    ANSEL1 = 1; //Set RA1 as analog input
    ADCON1Lbits.SSRC = 0b0111;   //Set Sampling to auto mode
        
    BUFREGEN = 1;          // Use buffer to stock the converted value
    ADCON3H = 0x9F;         //Using RC clock + 31TAD
    
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
	
    double sum = 0;

    //
    switch(channel){
        
        case VOLTAGE_CHANNEL:
            ADCHS0Lbits.CH0SA = 0; // Use AN0 
            for(int i = 0; i < AVERAGE_SAMPLES; i++)
            {
                SAMP = 1;           //Start the conversion
                while(DONE != 1){}  //Wait till its finished
                sum += ADCBUF0;     //Add to the sum
            }
            //Compute the average and return it
            return (uint16_t)(sum/AVERAGE_SAMPLES);
            break;
        case CURRENT_CHANNEL:
            ADCHS0Lbits.CH0SA = 1; // Use AN1
            for(int i = 0; i < AVERAGE_SAMPLES; i++)
            {
                SAMP = 1;           //Start the conversion
                while(DONE != 1){}  //Wait till its finished
                sum += ADCBUF1;     //Add to the sum
            }
            //Compute the average and return it
            return (uint16_t)(sum/AVERAGE_SAMPLES);
            break;
        default:
            break;
          
    }
    
    
}

/*This function call measure_adc(), format its return value and return the measured
 *voltage in millivolts */
uint16_t measure_voltage()
{
    return  ((uint32_t)measure_adc(VOLTAGE_CHANNEL)*ADC_REFH)/ADC_RESOLUTION;
}

/*This function call measure_adc(), format its return value and return the measured
 *voltage in millivolts */
uint16_t measure_current(uint16_t offset)
{
    //Computing the value in two stage to avoid overflow
    uint32_t temp = (((uint32_t)measure_adc(CURRENT_CHANNEL)*ADC_REFH)/ADC_RESOLUTION);
    temp = (uint32_t)((temp*1000/GAIN)/RESISTOR);
    
    //If the measured value is smaller than the offset, it return 0
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