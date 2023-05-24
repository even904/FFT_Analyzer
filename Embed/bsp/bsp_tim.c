#include "bsp_tim.h"

void tim2_init(void)
{
	HAL_TIM_Base_Start_IT(&htim2);
}


void tim3_init(void)
{
//	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_Base_Start_IT(&htim3);
}


