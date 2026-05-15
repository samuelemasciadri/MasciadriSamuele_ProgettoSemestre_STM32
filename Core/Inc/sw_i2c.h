/*************************************************************************//**
 *
 *    @file           sw_i2c.h
 *    @date           15.05.2026
 *    @version        0.1.0
 *
 *    @author         Samuele Masciadri
 *
 ******************************************************************************
 *
 *    @brief          Header file for software I2C driver.
 *
 *                    The software I2C bus is implemented using GPIO bit-banging:
 *                      - SDA on PF0
 *                      - SCL on PF1
 *
 ******************************************************************************/

#ifndef INC_SW_I2C_H_
#define INC_SW_I2C_H_

/* Public includes -----------------------------------------------------------*/
#include "main.h"
#include <stdint.h>

/* Public defines ------------------------------------------------------------*/

/* Public data types ---------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/

void sw_i2c_init(void);

uint8_t sw_i2c_probe(uint8_t address_7bit);
uint8_t sw_i2c_write(uint8_t address_7bit, uint8_t *data, uint16_t size);
uint8_t sw_i2c_read(uint8_t address_7bit, uint8_t *data, uint16_t size);

#endif /* INC_SW_I2C_H_ */

/****** END OF FILE ***********************************************************/
