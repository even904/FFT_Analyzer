#ifndef FUN_H
#define FUN_H

#include "stm32f1xx_hal.h"

void FFT_fun_init(void);
void FFT_fun_test(void);

#define  NPT  256             /* �������� */
extern uint32_t lBufInArray[NPT];    /* FFT ������������� */
extern uint32_t lBufOutArray[NPT/2]; /* FFT ������������ */
extern uint32_t lBufMagArray[NPT/2]; /* ����г���ķ�ֵ */

void adc_sample(void);
void adc_sample2(void);
void GetPowerMag(void);

void adc_load_fft_data(uint8_t ch);
#endif
