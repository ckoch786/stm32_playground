/**
  ******************************************************************************
  * @file    main.c
  * @author  Cory Koch
  * @version V1.0
  * @date    15-April-2025
  * @brief   Write hello world on the serial port.
  ******************************************************************************
*/

#include <stdbool.h>
#include "stm32f0xx.h"
#include "stm32f0xx_nucleo.h"

const char hello[] = "Hello World!\r\n";      // The message to send
int current; // The character in the message we are sending

UART_HandleTypeDef uartHandle;      // UART initialization

/**
 * @brief This function is executed in case of error occurrence.
 *
 * All it does is blink the LED.
 */
void Error_Handler(void)
{
	// TODO this does not get written to the UART, why?
	myPutchar("ERROR!!!!!!!!!!!!!!!!!");
	// Turn LED2 on.
	HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_SET);

	while (true)
	{
		// Toggle the state of LED2.
		HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
		HAL_Delay(1000); // 1 second
	}
}

/**
 * Send character to the UART.
 *
 * @param ch The character to send
 */
void myPutchar(const char ch)
{
	// This line gets and saves the value of UART_FLAG_TXE at call
	// time. This value changes so if you stop the program on the "if"
	// line below, the value will be set to zero because it goes away
	// faster than you can look at it.
	int result __attribute__((unused)) =
			(uartHandle.Instance->ISR && UART_FLAG_TXE);

	// Block until the transmit empty (TXE) flag is set.
	while ((uartHandle.Instance->ISR & UART_FLAG_TXE) == 0)
		continue;

	uartHandle.Instance->TDR = ch;       // Send character to the UART.
}

/**
 * Initialize LED2 (so we can blink red for error).
 */
void led2_Init(void)
{
	// LED clock initialization
	LED2_GPIO_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_LedInit;      // Initialization for the LED
	// Initialize LED.
	GPIO_LedInit.Pin = LED2_PIN;
	GPIO_LedInit.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_LedInit.Pull = GPIO_PULLUP;
	GPIO_LedInit.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_LedInit);
}

/**
 * Initialize UART2 for output.
 */
void uart2_Init(void)
{
	// UART initialization
	// UART2 -- one connected to ST-LINK USB
	uartHandle.Instance = USART2;
	uartHandle.Init.BaudRate = 9600;				 // Speed 9600
	uartHandle.Init.WordLength = UART_WORDLENGTH_8B; // 8 bits/character
	uartHandle.Init.StopBits = UART_PARITY_NONE;	 // 1 stop bit
	uartHandle.Init.Parity = UART_PARITY_NONE;		 // No parity
	uartHandle.Init.Mode = UART_MODE_TX_RX;			 // Transmit & receive
	uartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE; // No hw control

	// Over sample the incoming stream.
	uartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

	// Do not use one-bit sampling.
	uartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;

	// Nothing advanced
	uartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	/*
	 * For those of you connecting a terminal emulator, the above parameters
	 * translate to 9600,8,N,1.
	 */

	if (HAL_UART_Init(&uartHandle) != HAL_OK)
	{
		Error_Handler();
	}
}

int main(void)
{
	HAL_Init(); // Initialize hardware.
	led2_Init();
	uart2_Init();
	// Keep sending the message for a long time.
	for(;;) {
		// Send character by character.
		for(current = 0; hello[current] != '\0'; ++current) {
			myPutchar(hello[current]);
			// Error_Handler();
		}
		HAL_Delay(500);
	}
}

/**
 * Magic function that's called by the HAL layer to actually
 * initialize the UART.  In this case we need to
 * put the UART pins in alternate mode so they act as
 * UART pins and not like GPIO pins.
 *
 * @note: Only works for UART2, the one connected to the USB serial
 * converter
 *
 * @param uart the UART information
 */
void HAL_UART_MspInit(UART_HandleTypeDef* uart)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	if(uart->Instance == USART2)
	{
		// Peripheral clock enable
		__HAL_RCC_USART2_CLK_ENABLE();

		/*
		 * USART2 GPIO Configuration
		 * PA2     ------> USART2_TX
		 * PA3     ------> USART2_RX
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		// Alternate function -- that of UART
		GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}

/**
 * Magic function called by HAL layer to de-initialize the
 * USART hardware. It's something we never do, but we put it
 * in here for the sake of completeness.
 *
 * @note: Only works for UART2, the one connected to the USB serial
 * converter
 *
 * @param uart The UART information
 */
void HAL_UART_MspDeInit(UART_HandleTypeDef* uart)
{
	if(uart->Instance == USART2)
	{
		// Peripheral clock disable
		__HAL_RCC_USART2_CLK_DISABLE();

		/**
		 * USART2 GPIO Configuration
		 * PA2     -----> USART2_TX
		 * PA3     -----> USART2_RX
		 */
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);
	}
}






































