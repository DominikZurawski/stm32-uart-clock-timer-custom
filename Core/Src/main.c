#include "stm32l4xx_hal.h"
#include "main.h"
#include <string.h>

void SystemClock_Config(uint8_t);
void TIMER6_Init(void);
void GPIO_Init(void);
void Error_handler(void);

TIM_HandleTypeDef htimer6;

int main(void)
{
    HAL_Init();
    SystemClock_Config(SYS_CLOCK_FREQ_50_MHZ);
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

void SystemClock_Config(uint8_t clock_freq )
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	uint32_t FLatency =0;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	osc_init.HSIState = RCC_HSI_ON;
	osc_init.HSICalibrationValue = 16;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;

	switch(clock_freq) {
        case SYS_CLOCK_FREQ_50_MHZ:{
            osc_init.PLL.PLLM = 4;
            osc_init.PLL.PLLN = 25;
            osc_init.PLL.PLLR=2;
            osc_init.PLL.PLLP = 2;
            osc_init.PLL.PLLQ  = 2;

            clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
                                RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2;
            clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
            clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
            clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
            clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

            FLatency = FLASH_ACR_LATENCY_1WS;
            break;
    }
    case SYS_CLOCK_FREQ_80_MHZ: {
        osc_init.PLL.PLLM = 2;
        osc_init.PLL.PLLN = 20;
        osc_init.PLL.PLLR=2;
        osc_init.PLL.PLLP = 2;
        osc_init.PLL.PLLQ  = 2;

        clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
                            RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2;
        clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
        clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
        clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
        clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

        FLatency = FLASH_ACR_LATENCY_2WS;

        break;
    }

	default:
	  return;
	}

	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_handler();
	}


	if(HAL_RCC_ClockConfig(&clk_init,FLatency) != HAL_OK)
	{
		Error_handler();
	}

	//Systick configuration

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
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
    htimer6.Init.Prescaler = 9;
    htimer6.Init.Period = 50-1;

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

