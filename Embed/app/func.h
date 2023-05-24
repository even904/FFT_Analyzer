#ifndef FUN_H
#define FUN_H

#include "stm32f1xx_hal.h"

void FFT_fun_init(void);
void FFT_fun_test(void);

#define  NPT  256             /* 采样点数 */
extern uint32_t lBufInArray[NPT];    /* FFT 运算的输入数组 */
extern uint32_t lBufOutArray[NPT/2]; /* FFT 运算的输出数组 */
extern uint32_t lBufMagArray[NPT/2]; /* 各次谐波的幅值 */

void adc_sample(void);
void adc_sample2(void);
void GetPowerMag(void);

void adc_load_fft_data(uint8_t ch);
#endif
