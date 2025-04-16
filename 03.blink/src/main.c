/**
  ******************************************************************************
  * @file    main.c
  * @author  Cory Koch
  * @version V1.0
  * @date    14-April-2025
  * @brief   Blink the user LED on the board.
  * @detailed A simple program to write, but getting it working
  * is going to require learning a lot of tools.
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include "stm32f0xx_nucleo.h"
			

int main(void)
{
	HAL_Init();
	// LED clock initialization
	LED2_GPIO_CLK_ENABLE();

	// Initialize the LED
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = LED2_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);

	for(;;) {
		// Toggle LED2
		HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
		HAL_Delay(400); // Delay 400 ms
	}
}
