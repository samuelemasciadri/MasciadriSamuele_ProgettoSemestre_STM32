/*************************************************************************//**
 *
 *    @file           uart_debug.h
 *    @date           12.05.2026
 *    @version        0.2.0
 *
 *    @author         Samuele Masciadri
 *
 ******************************************************************************
 *
 *    @brief          Header file for UART debug command interface.
 *                    Commands are received through USART1 using interrupt mode.
 *
 ******************************************************************************/

#ifndef INC_UART_DEBUG_H_
#define INC_UART_DEBUG_H_

/* Public includes -----------------------------------------------------------*/
#include "main.h"

/* Public defines ------------------------------------------------------------*/

/* Public data types ---------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/

void uart_debug_init(void);

#endif /* INC_UART_DEBUG_H_ */

/****** END OF FILE ***********************************************************/
