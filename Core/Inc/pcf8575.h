/*************************************************************************//**
 *
 *    @file           pcf8575.h
 *    @date           15.05.2026
 *    @version        0.1.0
 *
 *    @author         Samuele Masciadri
 *
 ******************************************************************************
 *
 *    @brief          Header file for PCF8575 I2C GPIO expander driver.
 *
 ******************************************************************************/

#ifndef INC_PCF8575_H_
#define INC_PCF8575_H_

/* Public includes -----------------------------------------------------------*/
#include "main.h"
#include <stdint.h>

/* Public defines ------------------------------------------------------------*/

#define PCF8575_DEFAULT_ADDRESS     0x20U

/* Public data types ---------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/

void pcf8575_init(uint8_t address_7bit);

uint8_t pcf8575_scan(uint8_t *found_address);
uint8_t pcf8575_write(uint16_t value);
uint8_t pcf8575_read(uint16_t *value);
uint8_t pcf8575_toggle(void);
uint8_t pcf8575_read_int_pin(void);

#endif /* INC_PCF8575_H_ */

/****** END OF FILE ***********************************************************/
