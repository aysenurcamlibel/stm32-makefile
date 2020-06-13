#include <stdio.h>
#include "stm32f7xx_hal.h"
#include "stm32f7xx_nucleo_144.h"

static UART_HandleTypeDef UartHandle;

int __io_putchar(int ch);
int _write(int file,char *ptr, int len);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);

int main(void)
{
    /* Enable the CPU cache */
    CPU_CACHE_Enable();

    /* HAL initialization */
    if (HAL_Init() != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure the system clock to 216 MHz */
    SystemClock_Config();

    /* Enable GPIO clock */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* Configure LEDs */
    BSP_LED_Init(LED1);
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);

    /* Configure UART for printf */
    UartHandle.Instance          = USART3;
    UartHandle.Init.BaudRate     = 115200;
    UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits     = UART_STOPBITS_1;
    UartHandle.Init.Parity       = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode         = UART_MODE_TX_RX;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&UartHandle) != HAL_OK)
    {
        Error_Handler();
    }

    while (1)
    {
        printf("Hello World!\r\n");
        BSP_LED_Toggle(LED1);
        HAL_Delay(100);
    }
}

int __io_putchar(int ch)
{
    /* Support printf over UART */
    (void) HAL_UART_Transmit(&UartHandle, (uint8_t *) &ch, 1, 0xFFFFU);
    return ch;
}

int _write(int file, char *ptr, int len)
{
    /* Send chars over UART */
    for (int i = 0; i < len; i++)
    {
        (void) __io_putchar(*ptr++);
    }

    return len;
}

void SysTick_Handler(void)
{
    /* TODO: What does this do? Is it necessary? */
    HAL_IncTick();
}

static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    /* Enable power control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* Update the voltage scaling value */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 432;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 9;
    RCC_OscInitStruct.PLL.PLLR = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /* Activate the overdrive to reach the 216 MHz frequency */
    if (HAL_PWREx_EnableOverDrive() != HAL_OK)
    {
        Error_Handler();
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
    {
        Error_Handler();
    }
}

static void Error_Handler(void)
{
    /* Turn on a red LED */
    BSP_LED_On(LED3);

    /* Spin loop */
    while (1)
    {
        ;
    }
}

static void CPU_CACHE_Enable(void)
{
    /* Enable instruction cache */
    SCB_EnableICache();

    /* Enable data cache */
    SCB_EnableDCache();
}
