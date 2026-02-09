#ifndef BSP_ADC_H
#define BSP_ADC_H

#include <stdint.h>

#define ADC_NUM_CHANNELS  4

void adc_init(void);
void adc_start_scan(void);

#endif /* BSP_ADC_H */
