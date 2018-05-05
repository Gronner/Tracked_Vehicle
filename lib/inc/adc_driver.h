#ifndef __ADC_DRIVER_H_
#define __ADC_DRIVER_H_
/* @brief Initializes the pins, the ADC peripheral and two Channels */
void adc_init(void);

/* @brief Samples the two ADC Channels for motor voltage measurement
 * @param data Array for storing the channels samples
 */
void adc_sample(uint16_t data[]);
void adc_convert_sample(uint16_t data_in[], float data_out[]);
#endif // __ADC_DRIVER_H_
