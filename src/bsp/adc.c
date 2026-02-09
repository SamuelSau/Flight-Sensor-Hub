#include "adc.h"
#include "stm32f4xx.h"

void adc_init(void)
{
    /* Enable ADC1 clock (on APB2) */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    __asm volatile("dsb");

    /* Disable ADC before configuration */
    ADC1->CR2 = 0;

    /*
     * Common ADC configuration:
     * - Independent mode
     * - Prescaler /4 -> ADC clock = APB2/4 = 90/4 = 22.5 MHz (max 36 MHz)
     * - DMA mode: DMA requests issued on each conversion
     */
    ADC->CCR = ADC_CCR_ADCPRE_0;   /* /4 prescaler */

    /*
     * ADC1 configuration:
     * - 12-bit resolution (default)
     * - Scan mode enabled (multiple channels in sequence)
     * - DMA enabled, single DMA request mode (DDS=0 for single-scan)
     */
    ADC1->CR1 = ADC_CR1_SCAN;      /* Scan mode */
    ADC1->CR2 = ADC_CR2_DMA;       /* DMA mode, no DDS (single shot) */

    /*
     * Sequence configuration: 4 channels
     * SQR1[L] = 3 (4 conversions - 1)
     * SQR3: channels in order: IN0, IN1, IN4, IN5
     *
     * SQR3 bits:
     *   SQ1[4:0] = 0  (Channel 0 - altimeter)
     *   SQ2[4:0] = 1  (Channel 1 - accel X)
     *   SQ3[4:0] = 4  (Channel 4 - accel Y)
     *   SQ4[4:0] = 5  (Channel 5 - accel Z)
     */
    ADC1->SQR1 = (3U << ADC_SQR1_L_Pos);
    ADC1->SQR3 = (0U  << 0)     /* SQ1 = Channel 0 */
               | (1U  << 5)     /* SQ2 = Channel 1 */
               | (4U  << 10)    /* SQ3 = Channel 4 */
               | (5U  << 15);   /* SQ4 = Channel 5 */

    /*
     * Sampling time: 84 cycles for all channels (adequate for high-impedance inputs)
     * SMPR2 controls channels 0-9
     */
    ADC1->SMPR2 = (4U << (0 * 3))   /* Channel 0: 84 cycles */
                | (4U << (1 * 3))   /* Channel 1: 84 cycles */
                | (4U << (4 * 3))   /* Channel 4: 84 cycles */
                | (4U << (5 * 3));  /* Channel 5: 84 cycles */

    /* Enable ADC */
    ADC1->CR2 |= ADC_CR2_ADON;

    /* Wait for ADC to stabilize (TSTAB ~1us) */
    for (volatile int i = 0; i < 1000; i++);
}

void adc_start_scan(void)
{
    /* Re-enable DMA for this scan (DMA bit must be re-set for single mode) */
    ADC1->CR2 |= ADC_CR2_DMA;

    /* Re-enable DMA stream */
    DMA2_Stream0->NDTR = ADC_NUM_CHANNELS;
    DMA2_Stream0->CR |= DMA_SxCR_EN;

    /* Start conversion via software trigger */
    ADC1->CR2 |= ADC_CR2_SWSTART;
}
