#ifndef APP_TASK_H
#define APP_TASK_H

#include "stm32f1xx_hal.h"
#include "struct_typedef.h"
#include "bsp_tim.h"
#include "bsp_led.h"

extern volatile uint8_t TrigFlag;
extern uint8_t adc_flag;

typedef struct{
	enum {
		led_toggle_start,
		led_toggle_stop,
	}Task_led_toggle;
	enum {
		adc_sample_start,
		adc_sample_stop,
	}Task_adc_sample;
	enum {
		adc_fft_start,
		adc_fft_stop,
	}Task_adc_fft;
	enum {
		fft_display_start,
		fft_display_stop,
	}Task_fft_display;
	enum {
		default_display_start,
		default_display_stop,
	}Task_default_display;
	enum {
		usart_send_start,
		usart_send_stop,
	}Task_usart_send;
	enum {
		clean_screen_start,
		clean_screen_stop,
	}Task_clean_screen;
	enum {
		get_measurements_start,
		get_measurements_stop,
	}Task_get_measurements;
}TaskList_struct;

extern TaskList_struct TaskList;
extern uint16_t adc_sample_pos;

void TaskList_struct_init(TaskList_struct *TL);
void time_diplay(void);
void fft_diplay(void);
void time_wave_display(uint8_t ch);
void Draw_FFT_Cordinate(uint8_t type);
void Draw_FFT_Spectrum(uint8_t type);
void fft_display_spectrum(void);
void get_measurements_display(uint8_t ch,uint8_t en);
void clean_lcd_screen(void);
void show_status(void);
#endif
