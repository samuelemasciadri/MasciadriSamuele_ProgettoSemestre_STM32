/*************************************************************************//**
 *
 *    @file           tmp126.h
 *    @date           12.05.2026
 *    @version        0.1.0
 *
 *    @author         Samuele Masciadri
 *
 ******************************************************************************
 *
 *    @brief          Header file for TMP126 digital temperature sensor driver.
 *
 *                    The TMP126 is connected using a 3-wire SPI-like interface:
 *                      - CS  on PA3
 *                      - SCK on PA5
 *                      - SIO on PA6
 *
 *                    Communication is implemented with GPIO bit-banging because
 *                    the sensor uses a single bidirectional data line.
 *
 ******************************************************************************/

#ifndef INC_TMP126_H_
#define INC_TMP126_H_

/* Public includes -----------------------------------------------------------*/
#include "main.h"
#include <stdint.h>

/* Public defines ------------------------------------------------------------*/

/* Public data types ---------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/

void tmp126_init(void);

uint16_t tmp126_read_register(uint8_t reg_addr);
uint16_t tmp126_read_raw_temperature(void);
uint16_t tmp126_read_device_id(void);

int32_t tmp126_raw_to_centi_celsius(uint16_t raw);
float tmp126_raw_to_celsius(uint16_t raw);
float tmp126_read_temperature_c(void);

#endif /* INC_TMP126_H_ */

/****** END OF FILE ***********************************************************/
