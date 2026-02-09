#include "clock.h"
#include "stm32f4xx.h"

void clock_init(void)
{
    /* Enable HSI (should already be on after reset) */
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY));

    /* Set flash latency for 180 MHz: 5 wait states + prefetch + instruction cache + data cache */
    FLASH->ACR = FLASH_ACR_LATENCY_5WS
               | FLASH_ACR_PRFTEN
               | FLASH_ACR_ICEN
               | FLASH_ACR_DCEN;

    /* Enable power interface clock */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    __asm volatile("dsb");

    /* Set voltage regulator scale 1 (required for 180 MHz) */
    PWR->CR |= PWR_CR_VOS;

    /*
     * Configure PLL:
     *   Source  = HSI (16 MHz)
     *   PLLM   = 8   -> VCO input = 16/8 = 2 MHz
     *   PLLN   = 180  -> VCO output = 2 * 180 = 360 MHz
     *   PLLP   = 2   -> SYSCLK = 360/2 = 180 MHz
     *   PLLQ   = 8   -> USB = 360/8 = 45 MHz (not critical)
     */
    RCC->PLLCFGR = (8U  << RCC_PLLCFGR_PLLM_Pos)   /* PLLM = 8 */
                 | (180U << RCC_PLLCFGR_PLLN_Pos)   /* PLLN = 180 */
                 | (0U   << RCC_PLLCFGR_PLLP_Pos)   /* PLLP = 2 (00) */
                 | RCC_PLLCFGR_PLLSRC_HSI            /* HSI source */
                 | (8U   << RCC_PLLCFGR_PLLQ_Pos);  /* PLLQ = 8 */

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));

    /* Enable Over-drive mode for 180 MHz operation */
    PWR->CR |= PWR_CR_ODEN;
    while (!(PWR->CSR & PWR_CSR_ODRDY));

    PWR->CR |= PWR_CR_ODSWEN;
    while (!(PWR->CSR & PWR_CSR_ODSWRDY));

    /*
     * Configure bus clocks:
     *   AHB  prescaler = 1   -> HCLK  = 180 MHz
     *   APB1 prescaler = 4   -> PCLK1 =  45 MHz (max 45 MHz)
     *   APB2 prescaler = 2   -> PCLK2 =  90 MHz (max 90 MHz)
     */
    RCC->CFGR = RCC_CFGR_HPRE_DIV1
              | RCC_CFGR_PPRE1_DIV4
              | RCC_CFGR_PPRE2_DIV2;

    /* Switch system clock to PLL */
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    /* Update SystemCoreClock variable */
    SystemCoreClock = SYSCLK_HZ;
}
