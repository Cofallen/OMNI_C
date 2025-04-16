#include "bsp_buzzer.h"

static uint8_t buzzerSta = 0;

void BSP_Buzzer_Init(void)
{
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
}

void BSP_Buzzer_On(void)
{
    TIM4->CCR3 = 50;
    buzzerSta = 1;
}

void BSP_Buzzer_Off(void)
{
    TIM4->CCR3 = 0;
    buzzerSta = 0;
}

void BSP_Buzzer_Toggle(void)
{
    buzzerSta == 1 ? BSP_Buzzer_Off():BSP_Buzzer_On();
}
