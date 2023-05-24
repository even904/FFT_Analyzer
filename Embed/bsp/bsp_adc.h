#ifndef BSP_ADC_H
#define BSP_ADC_H

#include "stm32f1xx_hal.h"
#include "adc.h"

void ADC_init(void);
uint32_t get_adc_val(void);
void DMA_get_adc_val(void);
uint32_t get_internal_temp_adc_val(void);

extern volatile uint32_t Debug_adc_val;
extern volatile float adc_voltage;

extern uint32_t adc_dma_buf[2];
extern uint32_t ADC_CH0_BUF[256];
extern uint32_t ADC_CH1_BUF[256];
#endif
