#include "stm32l4xx_hal.h"
#include "main.h"
#include <string.h>

void SystemClockConfig(void);
void TIMER6_Init(void);
void GPIO_Init(void);
void Error_handler(void);

TIM_HandleTypeDef htimer6;

int main(void)
{
    HAL_Init();
    SystemClockConfig();
    GPIO_Init();
    TIMER6_Init();

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();

    //Start timer
    HAL_TIM_Base_Start_IT(&htimer6);

    while(1);
    return 0;
}

void SystemClockConfig(void)
{

}

void GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef ledgpio;
    ledgpio.Pin = GPIO_PIN_5;
    ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
    ledgpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA,&ledgpio);
}

void TIMER6_Init(void)
{
    htimer6.Instance = TIM6;
    htimer6.Init.Prescaler = 24;
    htimer6.Init.Period = 64000-1;

    if( HAL_TIM_Base_Init(&htimer6) != HAL_OK )
    {
        Error_handler();
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
}


void Error_handler(void)
{
    while(1);
}

