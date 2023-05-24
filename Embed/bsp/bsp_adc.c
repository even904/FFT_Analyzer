#include "bsp_adc.h"

ADC_ChannelConfTypeDef ADC_PA1_channel_config_struct =
{
	.Channel	  = ADC_CHANNEL_1,
	.Rank	 	  = ADC_REGULAR_RANK_1,
	.SamplingTime = ADC_SAMPLETIME_1CYCLE_5
};

ADC_ChannelConfTypeDef ADC_PA2_channel_config_struct =
{
	.Channel	  = ADC_CHANNEL_2,
	.Rank	 	  = ADC_REGULAR_RANK_2,
	.SamplingTime = ADC_SAMPLETIME_1CYCLE_5
};

ADC_ChannelConfTypeDef ADC_internal_temp_channel_config_struct =
{
	.Channel	  = ADC_CHANNEL_TEMPSENSOR,
	.Rank	 	  = ADC_REGULAR_RANK_2,
	.SamplingTime = ADC_SAMPLETIME_239CYCLES_5
};

void ADC_init(void)
{
//	HAL_ADC_ConfigChannel(&hadc1,&ADC_PA1_channel_config_struct);
//	HAL_ADC_ConfigChannel(&hadc1,&ADC_PA2_channel_config_struct);
}

static uint32_t get_adc_chx_val(ADC_HandleTypeDef hadcx,uint32_t Channel)
{
	uint32_t val = 0;
	static ADC_ChannelConfTypeDef ADC_config_struct;
	ADC_config_struct.Channel	   = Channel;
	ADC_config_struct.Rank	 	   = ADC_REGULAR_RANK_1;
	ADC_config_struct.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
	
	HAL_ADC_ConfigChannel(&hadc1,&ADC_config_struct);
	HAL_ADC_Start(&hadcx);
	HAL_ADC_PollForConversion(&hadcx,Channel);
	val = HAL_ADC_GetValue(&hadcx);
	return val;
}

uint32_t get_adc_val(void)
{
	uint32_t val = 0;
//	HAL_ADC_ConfigChannel(&hadc1,&ADC_PB1_channel_config_struct);
//	HAL_ADC_Start(&hadc1);
//	HAL_ADC_PollForConversion(&hadc1,ADC_CHANNEL_9);
//	val = HAL_ADC_GetValue(&hadc1);
	val = get_adc_chx_val(hadc1,ADC_CHANNEL_0);
	
	Debug_adc_val = val;
	adc_voltage = (float)val/4096*3.3;
	return val;
}

uint32_t get_internal_temp_adc_val(void)
{
	return get_adc_chx_val(hadc1,ADC_CHANNEL_16);
}


uint32_t adc_dma_buf[2];

uint32_t ADC_CH0_BUF[256];
uint32_t ADC_CH1_BUF[256];

static void DMA_get_adc_chx_val(ADC_HandleTypeDef hadcx)
{
//	HAL_ADC_Start(&hadcx);
	
	HAL_ADC_Start_DMA(&hadcx,(uint32_t *)adc_dma_buf,2);//PA1
	
//	HAL_ADC_Stop_DMA(&hadcx);
}

volatile uint32_t Debug_adc_val = 0;
volatile float adc_voltage = 0.0f;

void DMA_get_adc_val(void)
{
	DMA_get_adc_chx_val(hadc1);
}
