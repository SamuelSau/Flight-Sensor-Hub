#ifndef BSP_CLOCK_H
#define BSP_CLOCK_H

#include <stdint.h>

#define SYSCLK_HZ   180000000U
#define AHB_HZ       180000000U
#define APB1_HZ      45000000U
#define APB2_HZ      90000000U

void clock_init(void);

#endif /* BSP_CLOCK_H */
