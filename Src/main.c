/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stm32f1xx.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void GPIO_Config (void);
void Delay (uint32_t time);

uint32_t delayTime = 10000000;

int main(void)
{
	SystemInit();

	GPIO_Config();
	/* Loop forever */
	for(;;){
		uint8_t cur_state = (GPIOB->IDR & (1 << 1)); // read current state of PB1

		if(!cur_state) // if button is pushed
			delayTime /= 2; // speed up 2 times

		{
			// From HAL_GPIO_TogglePin
			uint16_t LED_PIN = (1 << 12);
			uint32_t LED0_mask = (GPIOB->ODR & LED_PIN);
			uint16_t LED_set_mask = ~LED0_mask & LED_PIN;
			uint16_t LED_reset_mask = LED0_mask & LED_PIN;

			GPIOB->BSRR |= (LED_set_mask | (LED_reset_mask << 16));
		}
		Delay(delayTime);
	}
}

void GPIO_Config (void)
{
	// Set IOPB EN
	RCC->APB2ENR |= (1 << 3); // Enable GPIOB clock

	// Set MODE1[1:0] = 00: : Input mode
	GPIOB->CRL &= ~(0b11 << 4); // clear bit 4 and 5

	// Clear  CNF1[1:0] = 01: Input floating
	GPIOB->CRL |= (0b11 << 6); // Set bit 6
	GPIOB->CRL &= ~(0b11 << 7); // Clear bit 7

	//
	// Set MODE12[1:0] = 01: : Output mode, max speed 10 MHz
	GPIOB->CRH |= (1 << 16); // set bit 16
	GPIOB->CRH &= ~(1 << 17); // clear bit 17

	// Clear  CNF12[1:0]: General purpose output push-pull
	GPIOB->CRH &= ~(0b11 << 18);
}

void Delay (uint32_t time)
{
	while (time--);
}
