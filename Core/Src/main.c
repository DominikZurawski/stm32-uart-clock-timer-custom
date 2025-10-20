#include <string.h>
#include "stm32l4xx_hal.h"
#include "main.h"

void GPIO_Init(void);
void Error_handler(void);
void TIMER2_Init(void);
void UART2_Init(void);
void SystemClock_Config_HSE(uint8_t clock_freq);

TIM_HandleTypeDef htimer2;
UART_HandleTypeDef huart2;

volatile uint32_t pulse1_value = 25000; //to produce 500Hz
volatile uint32_t pulse2_value = 12500; //to produce 1000HZ
volatile uint32_t pulse3_value = 6250;  //to produce 2000Hz
volatile uint32_t pulse4_value = 3125;  //to produce 4000Hz

volatile uint32_t ccr_content;

int main(void)
{
	HAL_Init();
	SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);
	GPIO_Init();
	UART2_Init();
	TIMER2_Init();

	if( HAL_TIM_OC_Start_IT(&htimer2,TIM_CHANNEL_1) != HAL_OK)
	{
		Error_handler();
	}

	if( HAL_TIM_OC_Start_IT(&htimer2,TIM_CHANNEL_2) != HAL_OK)
	{
		Error_handler();
	}

	if( HAL_TIM_OC_Start_IT(&htimer2,TIM_CHANNEL_3) != HAL_OK)
	{
		Error_handler();
	}

	if( HAL_TIM_OC_Start_IT(&htimer2,TIM_CHANNEL_4) != HAL_OK)
	{
		Error_handler();
	}

	while(1);

	return 0;
}

void SystemClock_Config_HSE(uint8_t clock_freq )
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;
    uint8_t FLatency=0;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE ;
	osc_init.HSEState = RCC_HSE_ON;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

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

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}

// void SystemClock_Config(uint8_t clock_freq )
// {
// 	RCC_OscInitTypeDef osc_init;
// 	RCC_ClkInitTypeDef clk_init;

// 	uint32_t FLatency =0;

// 	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE;
// 	osc_init.HSIState = RCC_HSI_ON;
// 	osc_init.LSEState = RCC_LSE_ON;
// 	osc_init.HSICalibrationValue = 16;
// 	osc_init.PLL.PLLState = RCC_PLL_ON;
// 	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;

// 	switch(clock_freq) {
//         case SYS_CLOCK_FREQ_50_MHZ:{
//             osc_init.PLL.PLLM = 4;
//             osc_init.PLL.PLLN = 25;
//             osc_init.PLL.PLLR=2;
//             osc_init.PLL.PLLP = 2;
//             osc_init.PLL.PLLQ  = 2;

//             clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
//                                 RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2;
//             clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//             clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
//             clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
//             clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

//             FLatency = FLASH_ACR_LATENCY_1WS;
//             break;
//     }
//     case SYS_CLOCK_FREQ_80_MHZ: {
//         osc_init.PLL.PLLM = 2;
//         osc_init.PLL.PLLN = 20;
//         osc_init.PLL.PLLR=2;
//         osc_init.PLL.PLLP = 2;
//         osc_init.PLL.PLLQ  = 2;

//         clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
//                             RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2;
//         clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//         clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
//         clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
//         clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

//         FLatency = FLASH_ACR_LATENCY_2WS;

//         break;
//     }

// 	default:
// 	  return;
// 	}

// 	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
// 	{
// 		Error_handler();
// 	}


// 	if(HAL_RCC_ClockConfig(&clk_init,FLatency) != HAL_OK)
// 	{
// 		Error_handler();
// 	}

// 	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
// 	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
// }

void GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef ledgpio;
    ledgpio.Pin = GPIO_PIN_5;
    ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
    ledgpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA,&ledgpio);
}

void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	if ( HAL_UART_Init(&huart2) != HAL_OK )
	{
		Error_handler();
	}
}

void TIMER2_Init(void)
{
  TIM_OC_InitTypeDef tim2OC_init;

  htimer2.Instance = TIM2;
  htimer2.Init.Period = 0XFFFFFFFF;
  htimer2.Init.Prescaler = 1;
  if ( HAL_TIM_OC_Init(&htimer2) != HAL_OK)
  {
    Error_handler();
  }

  tim2OC_init.OCMode = TIM_OCMODE_TOGGLE;
  tim2OC_init.OCPolarity = TIM_OCPOLARITY_HIGH;
  tim2OC_init.Pulse  = pulse1_value;
  if(HAL_TIM_OC_ConfigChannel(&htimer2,&tim2OC_init,TIM_CHANNEL_1) != HAL_OK)
  {
    Error_handler();
  }

  tim2OC_init.Pulse  = pulse2_value;
  if(HAL_TIM_OC_ConfigChannel(&htimer2,&tim2OC_init,TIM_CHANNEL_2) != HAL_OK)
  {
    Error_handler();
  }

  tim2OC_init.Pulse  = pulse3_value;
  if(HAL_TIM_OC_ConfigChannel(&htimer2,&tim2OC_init,TIM_CHANNEL_3) != HAL_OK)
  {
    Error_handler();
  }

  tim2OC_init.Pulse  = pulse4_value;
  if(HAL_TIM_OC_ConfigChannel(&htimer2,&tim2OC_init,TIM_CHANNEL_4) != HAL_OK)
  {
    Error_handler();
  }
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* TIM3_CH1 toggling with frequency = 500 Hz */
  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
  {
    ccr_content = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_1,ccr_content+pulse1_value);
  }

  /* TIM3_CH2 toggling with frequency = 1000 Hz */
  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
  {
    ccr_content = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
    __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_2,ccr_content+pulse2_value);
  }

  /* TIM3_CH3 toggling with frequency = 2000 Hz */
  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
  {
    ccr_content = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_3);
    __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_3,ccr_content+pulse3_value);
  }

  /* TIM3_CH4 toggling with frequency = 4000 Hz */
  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
  {
    ccr_content = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_4);
    __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_4,ccr_content+pulse4_value);
  }
}


void Error_handler(void)
{
    while(1);
}

