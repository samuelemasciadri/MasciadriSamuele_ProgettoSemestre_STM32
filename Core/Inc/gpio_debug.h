/*************************************************************************//**
 *
 *    @file           gpio_debug.h
 *    @date           05.05.2026
 *    @version        0.2.0
 *
 *    @author         Samuele Masciadri
 *
 ******************************************************************************
 *
 *    @brief          This file implements the header of the GPIO debug
 *                    module for LED_RED (PB7), LED_YELLOW (PB6) and
 *                    LED_GREEN (PB5) in the "Piattaforma didattica
 *                    ARM STM32F042K6Tx".
 *
 *    @section        environment Environment and Compiler Information
 *                    - STM32CubeIDE version: 1.19.0
 *                    - STM32F0 firmware version: 1.11.4
 *                    - Compiler: arm-gcc 11.6.1.202406241230
 *
 ******************************************************************************/

#ifndef INC_GPIO_DEBUG_H_
#define INC_GPIO_DEBUG_H_

/* Public includes -----------------------------------------------------------*/
#include "stm32f0xx.h"

/* Public defines ------------------------------------------------------------*/

/*
 * LED pin mapping (STM32F042K6Tx -- from .ioc file)
 *   LED_RED    -> PB7  (bit 7)
 *   LED_YELLOW -> PB6  (bit 6)
 *   LED_GREEN  -> PB5  (bit 5)
 *
 * All LEDs share the same port (GPIOB).
 *
 * ATTENZIONE:
 * I LED sono attivi bassi:
 *   - Livello LOW  (0) -> LED acceso
 *   - Livello HIGH (1) -> LED spento
 *
 * Macros write directly to BSRR/BRR/ODR registers -- no HAL overhead.
 */

#define LED_RED_PIN         (1U << 7U)
#define LED_YELLOW_PIN      (1U << 6U)
#define LED_GREEN_PIN       (1U << 5U)

#define LED_PORT            GPIOB

/* LED RED (PB7) */
#define LED_RED_ON()        (LED_PORT->BRR  = LED_RED_PIN)
#define LED_RED_OFF()       (LED_PORT->BSRR = LED_RED_PIN)
#define LED_RED_TOGGLE()    (LED_PORT->ODR ^= LED_RED_PIN)

/* LED YELLOW (PB6) */
#define LED_YELLOW_ON()     (LED_PORT->BRR  = LED_YELLOW_PIN)
#define LED_YELLOW_OFF()    (LED_PORT->BSRR = LED_YELLOW_PIN)
#define LED_YELLOW_TOGGLE() (LED_PORT->ODR ^= LED_YELLOW_PIN)

/* LED GREEN (PB5) */
#define LED_GREEN_ON()      (LED_PORT->BRR  = LED_GREEN_PIN)
#define LED_GREEN_OFF()     (LED_PORT->BSRR = LED_GREEN_PIN)
#define LED_GREEN_TOGGLE()  (LED_PORT->ODR ^= LED_GREEN_PIN)

/* All LEDs */
#define ALL_LEDS_OFF()      (LED_PORT->BSRR = LED_RED_PIN | LED_YELLOW_PIN | LED_GREEN_PIN)
#define ALL_LEDS_ON()       (LED_PORT->BRR  = LED_RED_PIN | LED_YELLOW_PIN | LED_GREEN_PIN)
#define ALL_LEDS_TOGGLE()   (LED_PORT->ODR ^= LED_RED_PIN | LED_YELLOW_PIN | LED_GREEN_PIN)

/* Public data types ---------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/

void gpio_debug_init(void);
void gpio_debug_section_check(void);
void gpio_debug_function_trace(void);

#endif /* INC_GPIO_DEBUG_H_ */
