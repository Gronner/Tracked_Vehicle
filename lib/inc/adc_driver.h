#ifndef __ADC_DRIVER_H_
#define __ADC_DRIVER_H_
#include <stdint.h>

/*! @brief Initializes the pins, the ADC peripheral and two Channels */
void adc_init(void);

/*! @brief Samples the two ADC Channels for motor voltage measurement
 * @param data Array for storing the channels samples
 */
void adc_sample(uint16_t data[]);

/*! @brief Converts sample data on the input array to volt
 * @param data_in Array of size 2 containing ADC values
 * @param data_out Array of size 2 determined for converted ACD values in volt
 */
void adc_convert_sample(uint16_t data_in[], float data_out[]);
#endif  // __ADC_DRIVER_H_
