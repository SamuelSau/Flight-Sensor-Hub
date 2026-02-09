#include "gpio.h"
#include "stm32f4xx.h"

void gpio_init(void)
{
    /* Enable GPIO clocks for ports A and D */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIODEN;
    __asm volatile("dsb");

    /*
     * PA0 (ADC1_IN0) - Analog mode (altimeter)
     * PA1 (ADC1_IN1) - Analog mode (accel X)
     * PA4 (ADC1_IN4) - Analog mode (accel Y)
     * PA5 (ADC1_IN5) - Analog mode (accel Z)
     *
     * MODER = 11 for analog mode
     */
    GPIOA->MODER |= GPIO_MODER_MODER0     /* PA0 analog */
                   | GPIO_MODER_MODER1     /* PA1 analog */
                   | GPIO_MODER_MODER4     /* PA4 analog */
                   | GPIO_MODER_MODER5;    /* PA5 analog */

    /*
     * USART2: PA2 (TX), PA3 (RX) - AF7
     * MODER = 10 for alternate function
     */
    GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
    GPIOA->MODER |=  (2U << GPIO_MODER_MODER2_Pos)
                   |  (2U << GPIO_MODER_MODER3_Pos);

    /* Set AF7 for PA2 and PA3 (USART2) via AFRL register */
    GPIOA->AFR[0] &= ~((0xFU << (2 * 4)) | (0xFU << (3 * 4)));
    GPIOA->AFR[0] |=  ((7U   << (2 * 4)) | (7U   << (3 * 4)));

    /* High speed for UART pins */
    GPIOA->OSPEEDR |= (3U << (2 * 2)) | (3U << (3 * 2));

    /* Push-pull, no pull-up/down for UART TX; pull-up for RX */
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR2 | GPIO_PUPDR_PUPDR3);
    GPIOA->PUPDR |= (1U << GPIO_PUPDR_PUPDR3);  /* Pull-up on RX */
}
