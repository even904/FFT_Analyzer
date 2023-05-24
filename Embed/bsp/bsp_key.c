#include "main.h"
#include "bsp_key.h"

static uint8_t key_status[5] = {0};

void key_init(void)
{
	uint8_t i = 0;
	for(i = 0;i < 5;i++)
	{
		key_status[i] = 0;
	}
}

void key_toggle_status(uint8_t i)
{
	if(key_status[i] == 1)
	{
		key_status[i] = 0;
	}
	else if(key_status[i] == 0)
	{
		key_status[i] = 1;
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
		if(KEY0 == KEY_PRESS){
			key_toggle_status(0);
			if (key_status[0] == 0){
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
			}
			if (key_status[0] == 1){
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
			}
//			HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
		}
		if(KEY1 == KEY_PRESS){
			key_toggle_status(1);
			HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);		
		}
		if(KEY2 == KEY_PRESS){
			key_toggle_status(2);
			HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
		}
		if(KEY3 == KEY_PRESS){
			key_toggle_status(3);
			HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
		}
		if(KEY4 == KEY_PRESS){
			key_toggle_status(4);
			HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
		}

}

uint8_t get_key_status(uint8_t i)
{
	return key_status[i];
}

