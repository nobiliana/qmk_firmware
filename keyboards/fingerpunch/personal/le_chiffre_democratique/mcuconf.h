/*
 * Copyright 2024 Sadek Baroudi <sadekbaroudi@gmail.com> (@sadekbaroudi)
 */

#pragma once

#include_next "mcuconf.h"

#if defined(CONVERT_TO_ELITE_PI) || defined(CONVERT_TO_RP2040_CE) || defined(CONVERT_TO_HELIOS) || defined(CONVERT_TO_LIATRIS)
#undef RP_I2C_USE_I2C1
#define RP_I2C_USE_I2C1 TRUE

#undef RP_PWM_USE_PWM7
#define RP_PWM_USE_PWM7 TRUE

#undef RP_PWM_USE_TIM1
#define RP_PWM_USE_TIM1 TRUE

#undef RP_SPI_USE_SPI0
#define RP_SPI_USE_SPI0 TRUE
#endif // CONVERT_TO_(any_rp2040)

#ifdef CONVERT_TO_STEMCELL
#undef STM32_SPI_USE_SPI0
#define STM32_SPI_USE_SPI0 TRUE

#undef STM32_SPI_USE_SPI1
#define STM32_SPI_USE_SPI1 TRUE

#undef STM32_ST_USE_TIMER
#define STM32_ST_USE_TIMER 5

#undef STM32_GPT_USE_TIM4
#define STM32_GPT_USE_TIM4 TRUE

#undef STM32_PWM_USE_TIM2
#define STM32_PWM_USE_TIM2 TRUE
#endif // CONVERT_TO_STEMCELL