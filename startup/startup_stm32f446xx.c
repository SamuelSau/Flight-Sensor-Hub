#include <stdint.h>
#include "stm32f4xx.h"

/* Linker script symbols */
extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

extern int main(void);

void Reset_Handler(void);
void Default_Handler(void);

/* Weak aliases for all IRQ handlers */
void NMI_Handler(void)         __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)   __attribute__((weak));
void MemManage_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void)  __attribute__((weak, alias("Default_Handler")));

/* FreeRTOS kernel handlers - forward to port */
void SVC_Handler(void)         __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)      __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)     __attribute__((weak, alias("Default_Handler")));

/* Peripheral IRQs - weak defaults */
void WWDG_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void PVD_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void TAMP_STAMP_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void RTC_WKUP_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void FLASH_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void RCC_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void EXTI0_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void EXTI1_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void EXTI2_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void EXTI3_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void EXTI4_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream0_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream1_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream2_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream3_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream4_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream5_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream6_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void ADC_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void CAN1_TX_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void CAN1_RX0_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void CAN1_RX1_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void CAN1_SCE_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void EXTI9_5_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void TIM1_BRK_TIM9_IRQHandler(void)      __attribute__((weak, alias("Default_Handler")));
void TIM1_UP_TIM10_IRQHandler(void)      __attribute__((weak, alias("Default_Handler")));
void TIM1_TRG_COM_TIM11_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void TIM2_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void TIM3_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void TIM4_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void I2C1_EV_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void I2C1_ER_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void I2C2_EV_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void I2C2_ER_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void SPI1_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void SPI2_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void EXTI15_10_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void RTC_Alarm_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void OTG_FS_WKUP_IRQHandler(void)        __attribute__((weak, alias("Default_Handler")));
void TIM8_BRK_TIM12_IRQHandler(void)     __attribute__((weak, alias("Default_Handler")));
void TIM8_UP_TIM13_IRQHandler(void)      __attribute__((weak, alias("Default_Handler")));
void TIM8_TRG_COM_TIM14_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM8_CC_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream7_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void FMC_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void SDIO_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void TIM5_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void SPI3_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void UART4_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void UART5_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void TIM6_DAC_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void TIM7_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream1_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream2_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream3_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream4_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void CAN2_TX_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void CAN2_RX0_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void CAN2_RX1_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void CAN2_SCE_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void OTG_FS_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream5_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream6_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream7_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void USART6_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void I2C3_EV_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void I2C3_ER_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void OTG_HS_EP1_OUT_IRQHandler(void)    __attribute__((weak, alias("Default_Handler")));
void OTG_HS_EP1_IN_IRQHandler(void)     __attribute__((weak, alias("Default_Handler")));
void OTG_HS_WKUP_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void OTG_HS_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void DCMI_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void FPU_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void SPI4_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void SAI1_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void SAI2_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void QuadSPI_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void HDMI_CEC_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void SPDIF_RX_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void FMPI2C1_EV_IRQHandler(void)        __attribute__((weak, alias("Default_Handler")));
void FMPI2C1_ER_IRQHandler(void)        __attribute__((weak, alias("Default_Handler")));

/* Vector table - must be placed at start of flash */
__attribute__((section(".isr_vector"), used))
void (* const g_pfnVectors[])(void) = {
    (void (*)(void))(&_estack),     /* Initial SP */
    Reset_Handler,                   /* Reset */
    NMI_Handler,                     /* NMI */
    HardFault_Handler,               /* Hard Fault */
    MemManage_Handler,               /* MPU Fault */
    BusFault_Handler,                /* Bus Fault */
    UsageFault_Handler,              /* Usage Fault */
    0, 0, 0, 0,                      /* Reserved */
    SVC_Handler,                     /* SVCall */
    DebugMon_Handler,                /* Debug Monitor */
    0,                               /* Reserved */
    PendSV_Handler,                  /* PendSV */
    SysTick_Handler,                 /* SysTick */
    /* External interrupts (IRQn 0..96) */
    WWDG_IRQHandler,                 /* 0 */
    PVD_IRQHandler,                  /* 1 */
    TAMP_STAMP_IRQHandler,           /* 2 */
    RTC_WKUP_IRQHandler,             /* 3 */
    FLASH_IRQHandler,                /* 4 */
    RCC_IRQHandler,                  /* 5 */
    EXTI0_IRQHandler,                /* 6 */
    EXTI1_IRQHandler,                /* 7 */
    EXTI2_IRQHandler,                /* 8 */
    EXTI3_IRQHandler,                /* 9 */
    EXTI4_IRQHandler,                /* 10 */
    DMA1_Stream0_IRQHandler,         /* 11 */
    DMA1_Stream1_IRQHandler,         /* 12 */
    DMA1_Stream2_IRQHandler,         /* 13 */
    DMA1_Stream3_IRQHandler,         /* 14 */
    DMA1_Stream4_IRQHandler,         /* 15 */
    DMA1_Stream5_IRQHandler,         /* 16 */
    DMA1_Stream6_IRQHandler,         /* 17 */
    ADC_IRQHandler,                  /* 18 */
    CAN1_TX_IRQHandler,              /* 19 */
    CAN1_RX0_IRQHandler,             /* 20 */
    CAN1_RX1_IRQHandler,             /* 21 */
    CAN1_SCE_IRQHandler,             /* 22 */
    EXTI9_5_IRQHandler,              /* 23 */
    TIM1_BRK_TIM9_IRQHandler,        /* 24 */
    TIM1_UP_TIM10_IRQHandler,        /* 25 */
    TIM1_TRG_COM_TIM11_IRQHandler,   /* 26 */
    TIM1_CC_IRQHandler,              /* 27 */
    TIM2_IRQHandler,                 /* 28 */
    TIM3_IRQHandler,                 /* 29 */
    TIM4_IRQHandler,                 /* 30 */
    I2C1_EV_IRQHandler,              /* 31 */
    I2C1_ER_IRQHandler,              /* 32 */
    I2C2_EV_IRQHandler,              /* 33 */
    I2C2_ER_IRQHandler,              /* 34 */
    SPI1_IRQHandler,                 /* 35 */
    SPI2_IRQHandler,                 /* 36 */
    Default_Handler,                 /* 37: USART1 */
    USART2_IRQHandler,               /* 38 */
    Default_Handler,                 /* 39: USART3 */
    EXTI15_10_IRQHandler,            /* 40 */
    RTC_Alarm_IRQHandler,            /* 41 */
    OTG_FS_WKUP_IRQHandler,          /* 42 */
    TIM8_BRK_TIM12_IRQHandler,       /* 43 */
    TIM8_UP_TIM13_IRQHandler,        /* 44 */
    TIM8_TRG_COM_TIM14_IRQHandler,   /* 45 */
    TIM8_CC_IRQHandler,              /* 46 */
    DMA1_Stream7_IRQHandler,         /* 47 */
    FMC_IRQHandler,                  /* 48 */
    SDIO_IRQHandler,                 /* 49 */
    TIM5_IRQHandler,                 /* 50 */
    SPI3_IRQHandler,                 /* 51 */
    UART4_IRQHandler,                /* 52 */
    UART5_IRQHandler,                /* 53 */
    TIM6_DAC_IRQHandler,             /* 54 */
    TIM7_IRQHandler,                 /* 55 */
    DMA2_Stream0_IRQHandler,         /* 56 */
    DMA2_Stream1_IRQHandler,         /* 57 */
    DMA2_Stream2_IRQHandler,         /* 58 */
    DMA2_Stream3_IRQHandler,         /* 59 */
    DMA2_Stream4_IRQHandler,         /* 60 */
    0, 0,                            /* 61-62: Reserved */
    CAN2_TX_IRQHandler,              /* 63 */
    CAN2_RX0_IRQHandler,             /* 64 */
    CAN2_RX1_IRQHandler,             /* 65 */
    CAN2_SCE_IRQHandler,             /* 66 */
    OTG_FS_IRQHandler,               /* 67 */
    DMA2_Stream5_IRQHandler,         /* 68 */
    DMA2_Stream6_IRQHandler,         /* 69 */
    DMA2_Stream7_IRQHandler,         /* 70 */
    USART6_IRQHandler,               /* 71 */
    I2C3_EV_IRQHandler,              /* 72 */
    I2C3_ER_IRQHandler,              /* 73 */
    OTG_HS_EP1_OUT_IRQHandler,       /* 74 */
    OTG_HS_EP1_IN_IRQHandler,        /* 75 */
    OTG_HS_WKUP_IRQHandler,          /* 76 */
    OTG_HS_IRQHandler,               /* 77 */
    DCMI_IRQHandler,                 /* 78 */
    0,                               /* 79: Reserved */
    0,                               /* 80: Reserved */
    FPU_IRQHandler,                  /* 81 */
    0, 0,                            /* 82-83: Reserved */
    SPI4_IRQHandler,                 /* 84 */
    0, 0,                            /* 85-86: Reserved */
    SAI1_IRQHandler,                 /* 87 */
    0, 0, 0,                         /* 88-90: Reserved */
    SAI2_IRQHandler,                 /* 91 */
    QuadSPI_IRQHandler,              /* 92 */
    HDMI_CEC_IRQHandler,             /* 93 */
    SPDIF_RX_IRQHandler,             /* 94 */
    FMPI2C1_EV_IRQHandler,           /* 95 */
    FMPI2C1_ER_IRQHandler,           /* 96 */
};

void __attribute__((noreturn)) HardFault_Handler(void)
{
    while (1) {
        __asm volatile("nop");
    }
}

void __attribute__((noreturn)) Default_Handler(void)
{
    while (1) {
        __asm volatile("nop");
    }
}

void __attribute__((noreturn)) Reset_Handler(void)
{
    /* Copy .data from Flash to SRAM */
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    /* Zero .bss */
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    /* Enable FPU: set CP10 and CP11 to Full Access */
    SCB->CPACR |= ((3U << 10*2) | (3U << 11*2));
    __asm volatile("dsb");
    __asm volatile("isb");

    main();

    while (1);
}
