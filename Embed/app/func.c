#include "arm_math.h"  //���ͷ�ļ�
#include "func.h"
#include "stm32_dsp.h"
#include "bsp_adc.h"

#define  SAMPLE_FREQ       256       //����Ƶ��

uint32_t lBufInArray[NPT];    /* FFT ������������� */
uint32_t lBufOutArray[NPT/2]; /* FFT ������������ */
uint32_t lBufMagArray[NPT/2]; /* ����г���ķ�ֵ */

void adc_sample(void)
{
//	uint32_t i = 0;
//	for (i = 0; i < NPT; i++)
//	{
////		lBufInArray[i] = ADC_ConvertedValue[i];
//		lBufInArray[i] = (uint32_t)(20 + 10*arm_sin_f32(2*PI*(float)i*10/(float)SAMPLE_FREQ) + 5.5*arm_sin_f32(2*PI*(float)i*50/(float)SAMPLE_FREQ));
//		lBufInArray[i] = lBufInArray[i] & 0x00FF;
//	}
//	cr4_fft_256_stm32(lBufOutArray,lBufInArray, NPT);
//	GetPowerMag();
}

void adc_sample2(void)
{
//	uint32_t i = 0;
//	for (i = 0; i < NPT; i++)
//	{
////		lBufInArray[i] = ADC_ConvertedValue[i];
//		lBufInArray[i] = (uint32_t)(20 + 10*arm_sin_f32(2*PI*(float)i*30/(float)SAMPLE_FREQ) + 5.5*arm_sin_f32(2*PI*(float)i*70/(float)SAMPLE_FREQ));
//		lBufInArray[i] = lBufInArray[i] & 0x00FF;
//	}
//	cr4_fft_256_stm32(lBufOutArray,lBufInArray, NPT);
//	GetPowerMag();
}

/*
 * Load FFT data from adc buffer and Start FFT 
 * ch = 0: channel0;1:channel 1
**/
void adc_load_fft_data(uint8_t ch)
{
	uint32_t i = 0;
	
	if(ch == 0)
	{
		for (i = 0; i < NPT; i++)
		{
			lBufInArray[i] = ADC_CH0_BUF[i] & 0x0000FFFF;
		}
	}
	if(ch == 1)
	{
		for (i = 0; i < NPT; i++)
		{
			lBufInArray[i] = ADC_CH1_BUF[i] & 0x0000FFFF;
		}
	}
	cr4_fft_256_stm32(lBufOutArray,lBufInArray, NPT);
	GetPowerMag();
}


void GetPowerMag(void)
{
    signed short lX,lY;
    float X,Y,Mag;
    unsigned short i;
    for(i=0; i<NPT/2; i++)
    {
        lX  = (lBufOutArray[i] << 16) >> 16;
        lY  = (lBufOutArray[i] >> 16);
			
		//����32768�ٳ�65536��Ϊ�˷��ϸ������������
        X = NPT * ((float)lX) / 32768;
        Y = NPT * ((float)lY) / 32768;
        Mag = sqrt(X * X + Y * Y)*1.0/ NPT;
        if(i == 0)	
            lBufMagArray[i] = (unsigned long)(Mag * 32768);
        else
            lBufMagArray[i] = (unsigned long)(Mag * 65536);
    }
}






