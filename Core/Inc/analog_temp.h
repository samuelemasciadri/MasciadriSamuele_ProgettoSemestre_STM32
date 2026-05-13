/*************************************************************************//**
 *
 *    @file           analog_temp.h
 *    @date           13.05.2026
 *    @version        0.1.0
 *
 *    @author         Samuele Masciadri
 *
 ******************************************************************************
 *
 *    @brief          Header file for analog temperature sensor driver.
 *
 *                    The analog temperature sensor is connected to PA0,
 *                    configured as ADC_IN0.
 *
 ******************************************************************************/

#ifndef INC_ANALOG_TEMP_H_
#define INC_ANALOG_TEMP_H_

/* Public includes -----------------------------------------------------------*/
#include "main.h"
#include <stdint.h>

/* Public defines ------------------------------------------------------------*/

/* Public data types ---------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/

void analog_temp_init(ADC_HandleTypeDef *hadc);

uint32_t analog_temp_read_raw(void);
uint32_t analog_temp_raw_to_millivolts(uint32_t raw);
int32_t analog_temp_millivolts_to_centi_celsius(uint32_t voltage_mv);

float analog_temp_read_voltage(void);
float analog_temp_read_lm235_celsius(void);

#endif /* INC_ANALOG_TEMP_H_ */

/****** END OF FILE ***********************************************************/
