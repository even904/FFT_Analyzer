#include "app_task.h"
#include "func.h"
#include "lcd_gui.h"
#include "bsp_adc.h"
#include "lcd.h"
#include "bsp_key.h"

TaskList_struct TaskList;//定义任务列表，模仿任务句柄

void TaskList_struct_init(TaskList_struct *TL)
{
	TL->Task_led_toggle = led_toggle_stop;
	TL->Task_adc_sample = adc_sample_stop;
	TL->Task_adc_fft	= adc_fft_stop;
	TL->Task_fft_display= fft_display_stop;
	TL->Task_default_display = default_display_stop;
	TL->Task_usart_send = usart_send_stop;
	TL->Task_clean_screen 	  = clean_screen_stop;
	TL->Task_get_measurements = get_measurements_stop;
}



uint16_t adc_sample_pos = 0;

volatile uint32_t time_flag = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim2.Instance)
	{
		/* 256Hz Sampling Task */
		TaskList.Task_adc_sample = adc_sample_start;
		if(adc_sample_pos == NPT)
		{
			adc_sample_pos = 0;
			/* 0:normal display 1: stop */
			if(get_key_status(2) == 0)
			{
				TaskList.Task_adc_fft = adc_fft_start;
				TaskList.Task_fft_display = fft_display_start;
			}
		}
	}
	if(htim->Instance == htim3.Instance)
	{

		/* Conditional Task */
		if(get_key_status(4)){
			TaskList.Task_clean_screen = clean_screen_start;
			key_toggle_status(4);
		}
		
		/* 50ms Task */
		if(time_flag%50 == 0)
		{
			TaskList.Task_led_toggle = led_toggle_start;
		}
		
		/* 20ms Task */
		if(time_flag%30 == 0)
		{
			/* Conditional Task */
			if(get_key_status(3)){
				TaskList.Task_get_measurements = get_measurements_start;
			}
			else{
				TaskList.Task_get_measurements = get_measurements_stop;
			}
			
			TaskList.Task_default_display = default_display_start;
//			TaskList.Task_usart_send	  = usart_send_start;
		}
		
		time_flag++;
	}
	
}

void fft_diplay(void)
{
	int m = 0,n = 0;
	for(n = 0;n < 10 ; n++)
	{
		for(m = 0;m < 10;m++)
		 {
			 LCD_ShowNum(m*24,80+(16*n),lBufMagArray[m+n*10],4,12);
		 }
	}
}

void time_diplay(void)
{
	int m = 0,n = 0;
	for(n = 0;n < 10 ; n++)
	{
		for(m = 0;m < 10;m++)
		 {
			 LCD_ShowNum(m*24,80+(16*n),ADC_CH0_BUF[m+n*10],4,12);
		 }
	}
}

#define TIME_H 100
#define TIME_X 0
#define TIME_Y 239-120
#define TIME_MES_TAB_WIDTH	63
uint16_t Time_Wave_Buf[256] = {0};
void time_wave_display(uint8_t ch)
{

	/* Clean Display Area */
	LCD_Fill(TIME_X,TIME_Y - TIME_H,TIME_X+256,TIME_Y,BACK_COLOR);
	/* Draw Wire Frames */
	GUI_DrawLine(TIME_X + 256,TIME_Y - TIME_H,TIME_X + 256,TIME_Y,BLACK);
	LCD_DrawRectangle(TIME_X,TIME_Y - TIME_H - 1 ,TIME_X + 256 + TIME_MES_TAB_WIDTH , TIME_Y + 1);
	uint16_t k = 0;
	/* Time Wave Normalization */
	if(ch == 0)
	{
		uint32_t max_mag = ADC_CH0_BUF[0];
		for(k = 0;k < 256;k++)
		{
			if(max_mag < ADC_CH0_BUF[k])
			{
				max_mag = ADC_CH0_BUF[k];
			}
		}
		for(k = 0;k < 256;k++)
		{
			Time_Wave_Buf[k] = ADC_CH0_BUF[k]*TIME_H/max_mag;
		}
	}
	if(ch == 1)
	{
		uint32_t max_mag = ADC_CH1_BUF[0];
		for(k = 0;k < 256;k++)
		{
			if(max_mag < ADC_CH1_BUF[k])
			{
				max_mag = ADC_CH1_BUF[k];
			}
		}
		for(k = 0;k < 256;k++)
		{
			Time_Wave_Buf[k] = ADC_CH1_BUF[k]*TIME_H/max_mag;
		}
	}

	/* Time Wave Display */
	uint16_t m,i = 0;
	for(m = 0;m < 256;m++)
	{
		GUI_DrawLine(TIME_X+m,TIME_Y-Time_Wave_Buf[i],TIME_X+m,TIME_Y,BLUE);
		i++;
	}
}


#define FFT_H 100 //Spectrum Proportion of Screen
#define ORIGIN_X 0
#define ORIGIN_Y 239-16
#define SPECTRUM_DISPLAY_MODE 1 //0:width 160;1:width 320
uint16_t Spectrum_Buf[NPT/2] = {0};
void fft_display_spectrum(void)
{
	/* Clean Display Area */
	LCD_Fill(ORIGIN_X,ORIGIN_Y - FFT_H,ORIGIN_X+(SPECTRUM_DISPLAY_MODE+1)*160,ORIGIN_Y + 16,BACK_COLOR);
	
	/* Spectrum Normalization */
	uint32_t max_mag = lBufMagArray[0];
	uint16_t k = 0;
	for(k = 0;k < NPT/2;k++)
	{
		if(max_mag < lBufMagArray[k])
		{
			max_mag = lBufMagArray[k];
		}
	}
	for(k = 0;k < NPT/2;k++)
	{
		Spectrum_Buf[k] = lBufMagArray[k]*FFT_H/max_mag;
	}
	/* Reference Cordinate |_*/
	Draw_FFT_Cordinate(SPECTRUM_DISPLAY_MODE);

	/* FFT Spectrum */
	Draw_FFT_Spectrum(SPECTRUM_DISPLAY_MODE);
}

/* 
 * type 0: valid figure window width 128 total 160
 * type 1: valid figure window width 256 total 320
 */
void Draw_FFT_Cordinate(uint8_t type)
{
	//Set windows in 320*240 horizon mode,without what couldn't display in full screen.	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);
	uint16_t k = 0;
	
	if (type == 0)
	{
		
		for(k = 0;k <= NPT/20+1;k+=2)
		{
			/* Scale Mark */
			LCD_ShowNum(ORIGIN_X+k*10-11,ORIGIN_Y,k*10,3,12);
			/* | Vertical Ordinate */
			GUI_DrawLine(ORIGIN_X+k*10,ORIGIN_Y-FFT_H,ORIGIN_X+k*10,ORIGIN_Y,GRAY);
		}
			/* -- Horizonal Ordinate */
		for(k = 0;k < FFT_H/10+1;k++)
		{
			GUI_DrawLine(ORIGIN_X,ORIGIN_Y-k*10,ORIGIN_X+(NPT/20+1)*10,ORIGIN_Y-k*10,GRAY);
		}
		
		/* Data Watch Window */
		LCD_DrawRectangle(ORIGIN_X+(NPT/20+1)*10,ORIGIN_Y-(k-1)*10,ORIGIN_X+(NPT/20+1)*10+29,ORIGIN_Y-(k-1)*10+FFT_H);
	}
	if (type == 1)
	{
		/* Scale Mark */
		for(k = 0;k <= NPT/20+1;k++)
		{
			/* Scale Mark */
			LCD_ShowNum(ORIGIN_X+k*20-11,ORIGIN_Y,k*10,3,12);
			/* | Vertical Ordinate */
			GUI_DrawLine(ORIGIN_X+k*20,ORIGIN_Y-FFT_H,ORIGIN_X+k*20,ORIGIN_Y,GRAY);
		}
			/* Legend */
		LCD_ShowString(ORIGIN_X+k*20,ORIGIN_Y,12,"/Hz",0);
			/* -- Horizonal Ordinate */
		for(k = 0;k < FFT_H/10+1;k++)
		{
			GUI_DrawLine(ORIGIN_X,ORIGIN_Y-k*10,ORIGIN_X+(NPT/20+1)*20,ORIGIN_Y-k*10,GRAY);
		}
		
		/* Data Watch Window */
		LCD_DrawRectangle(ORIGIN_X+(NPT/20+1)*20,ORIGIN_Y-(k-1)*10,ORIGIN_X+(NPT/20+1)*20+59,ORIGIN_Y-(k-1)*10+FFT_H);
	}
}


void Draw_FFT_Spectrum(uint8_t type)
{
	uint16_t m,i = 0;
	for(m = 0;m < NPT/2;m++)
	{
		GUI_DrawLine(ORIGIN_X+m*2,ORIGIN_Y-Spectrum_Buf[i],ORIGIN_X+m*2,ORIGIN_Y,RED);
		i++;
	}
}

void get_measurements_display(uint8_t ch,uint8_t en)
{
	if(en == 1)
	{
		/**** Time Wave Measurements ****/
		LCD_ShowString(TIME_X + 256 + 5,TIME_Y - TIME_H,12,"Time Wave",0);
		/* 1. Get Maximium/Minimum/Average Amplitude */
		uint16_t k = 0,value_display = 0;;
		if(ch == 0)
		{
			uint32_t max_amp = ADC_CH0_BUF[0],min_amp = ADC_CH0_BUF[0];
			uint32_t sum = ADC_CH0_BUF[0];
			for(k = 0;k < 256;k++)
			{
				sum += ADC_CH0_BUF[k];//cal sum
				if(max_amp < ADC_CH0_BUF[k])//get max amp
				{max_amp = ADC_CH0_BUF[k];}
				if(min_amp > ADC_CH0_BUF[k])//get min amp
				{min_amp = ADC_CH0_BUF[k];}
			}
			/*max*/value_display = max_amp*3300/4096;
			LCD_ShowString(TIME_X + 256 + 5,TIME_Y - TIME_H + 12,12,"Max",0);
			LCD_ShowNum(TIME_X + 256 + 24,TIME_Y - TIME_H + 12,value_display,4,12);
			/*min*/value_display = min_amp*3300/4096;
			LCD_ShowString(TIME_X + 256 + 5,TIME_Y - TIME_H + 24,12,"Min",0);
			LCD_ShowNum(TIME_X + 256 + 24,TIME_Y - TIME_H + 24,value_display,4,12);
			/*average*/value_display = sum/256*3300/4096;
			LCD_ShowString(TIME_X + 256 + 5,TIME_Y - TIME_H + 36,12,"Av.",0);
			LCD_ShowNum(TIME_X + 256 + 24,TIME_Y - TIME_H + 36,value_display,4,12);
		}
		if(ch == 1)
		{
			uint32_t max_amp = ADC_CH1_BUF[0],min_amp = ADC_CH1_BUF[0];
			uint32_t sum = ADC_CH1_BUF[0];
			for(k = 0;k < 256;k++)
			{
				sum += ADC_CH1_BUF[k];//cal sum
				if(max_amp < ADC_CH1_BUF[k])//get max amp
				{max_amp = ADC_CH1_BUF[k];}
				if(min_amp > ADC_CH1_BUF[k])//get min amp
				{min_amp = ADC_CH1_BUF[k];}
			}
			/*max*/value_display = max_amp*3300/4096;
			LCD_ShowString(TIME_X + 256 + 5,TIME_Y - TIME_H + 12,12,"Max",0);
			LCD_ShowNum(TIME_X + 256 + 24,TIME_Y - TIME_H + 12,value_display,4,12);
			/*min*/value_display = min_amp*3300/4096;
			LCD_ShowString(TIME_X + 256 + 5,TIME_Y - TIME_H + 24,12,"Min",0);
			LCD_ShowNum(TIME_X + 256 + 24,TIME_Y - TIME_H + 24,value_display,4,12);
			/*average*/value_display = sum/256*3300/4096;
			LCD_ShowString(TIME_X + 256 + 5,TIME_Y - TIME_H + 36,12,"Av.",0);
			LCD_ShowNum(TIME_X + 256 + 24,TIME_Y - TIME_H + 36,value_display,4,12);
		}
		
		LCD_ShowString(TIME_X + 256 + 5,TIME_Y - TIME_H + 60,12,"Unit:mV",0);
		LCD_ShowString(TIME_X + 256 + 5,TIME_Y - TIME_H + 72,12,"2023/5/3",0);
		/**** FFT Measurements ****/
		LCD_ShowString(ORIGIN_X + 2*131,ORIGIN_Y-FFT_H+1,12,"FFT (mV)",0);
		/* 1. Get DC/MaxAC Amplitude */

		uint32_t maxac_amp =  lBufMagArray[1];
		uint16_t maxac_index = 1;
		for(k = 1;k < NPT/2;k++)
		{
			if(maxac_amp < lBufMagArray[k])
			{
				maxac_amp = lBufMagArray[k];
				maxac_index = k;
			}
		}
		/*DCamp*/value_display = lBufMagArray[0]*3300/4096;//Test Coefficiency
		LCD_ShowString(ORIGIN_X + 2*131,ORIGIN_Y-FFT_H+1+12,12,"DCamp",0);
		LCD_ShowNum(ORIGIN_X + 2*131+32,ORIGIN_Y-FFT_H+1+12,value_display,4,12);
		/*MaxACamp*/value_display = maxac_amp*3300/4096;
		LCD_ShowString(ORIGIN_X + 2*131,ORIGIN_Y-FFT_H+1+24,12,"MaxAC",0);
		LCD_ShowNum(ORIGIN_X + 2*131+32,ORIGIN_Y-FFT_H+1+24,value_display,4,12);
		/*MaxACFreq*/
		LCD_ShowNum(ORIGIN_X + 2*131 ,ORIGIN_Y-FFT_H+1+36,maxac_index,3,12);
		LCD_ShowString(ORIGIN_X + 2*131 + 36,ORIGIN_Y-FFT_H+1+36,12,"Hz",0);
		
		LCD_ShowString(ORIGIN_X + 2*131,ORIGIN_Y-FFT_H+1+48,12,"Yinnan",0);
		LCD_ShowString(ORIGIN_X + 2*131,ORIGIN_Y-FFT_H+1+60,12,"Zhang",0);
		LCD_ShowString(ORIGIN_X + 2*131,ORIGIN_Y-FFT_H+1+72,12,"20200702",0);
		LCD_ShowString(ORIGIN_X + 2*131,ORIGIN_Y-FFT_H+1+84,12,"0129",0);
	}
	else if(en == 0)
	{
		/* Clean Time Wave Measurements */
		LCD_SetWindows(TIME_X + 256 + 5,TIME_Y - TIME_H,TIME_X + 256 + TIME_MES_TAB_WIDTH , TIME_Y + 1);
		LCD_Fill(TIME_X + 256 + 5,TIME_Y - TIME_H,TIME_X + 256 + TIME_MES_TAB_WIDTH , TIME_Y + 1,BACK_COLOR);
		/* Clean FFT Measurements */
		LCD_SetWindows(ORIGIN_X + 2*131,ORIGIN_Y-FFT_H+1,ORIGIN_X+(NPT/20+1)*20+59,ORIGIN_Y-1);
		LCD_Fill(ORIGIN_X + 2*131,ORIGIN_Y-FFT_H+1,ORIGIN_X+(NPT/20+1)*20+59,ORIGIN_Y-1,BACK_COLOR);
		LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);
	}
}

void clean_lcd_screen(void)
{
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);
	LCD_Fill(0,0,lcddev.width,lcddev.height-1,BACK_COLOR);
}

void show_status(void)
{
	LCD_ShowString(162,0,16,"CH:",0);
	LCD_ShowNum(190,0,get_key_status(1),1,16);
	
	if(get_key_status(2) == 0)
	{LCD_ShowString(220,0,16,"START",0);}
	else if(get_key_status(2) == 1)
	{LCD_ShowString(220,0,16,"STOP ",0);}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{	
	ADC_CH0_BUF[adc_sample_pos] = adc_dma_buf[0];
	ADC_CH1_BUF[adc_sample_pos] = adc_dma_buf[1];
	adc_sample_pos++;//Finish adc sample,pos increment;
	TaskList.Task_adc_sample = adc_sample_stop;
	HAL_ADC_Stop_DMA(&hadc1); 
}
