/*************************************************************************//**
 *
 *    @file           uart_debug.c
 *    @date           05.05.2026
 *    @version        0.1.0
 *
 *    @author         Samuele Masciadri
 *
 ******************************************************************************
 *
 *    @brief          UART debug command interface for STM32F042K6Tx.
 *
 *                    Communication settings:
 *                      - USART1
 *                      - 9600 baud
 *                      - 8 data bits
 *                      - No parity
 *                      - 1 stop bit
 *
 *                    RX is managed by interrupt, therefore the main while(1)
 *                    loop can remain empty.
 *
 *                    Supported commands:
 *                      - redLED_on
 *                      - greenLED_on
 *                      - yellowLED_on
 *                      - redLED_off
 *                      - greenLED_off
 *                      - yellowLED_off
 *                      - toggleLEDs
 *                      - readADC
 *                      - readSPI_TMP
 *                      - readI2C
 *
 ******************************************************************************/

/* Private includes ----------------------------------------------------------*/
#include "uart_debug.h"
#include "gpio_debug.h"
#include <string.h>

/* Private defines -----------------------------------------------------------*/

#define UART_RX_BUFFER_SIZE     32U

/* Private data types --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

extern UART_HandleTypeDef huart1;

static uint8_t rx_byte = 0U;
static char rx_buffer[UART_RX_BUFFER_SIZE];
static uint8_t rx_index = 0U;

/* Private function prototypes -----------------------------------------------*/

static void uart_debug_send_string(const char *msg);
static void uart_debug_process_command(char *cmd);
static void uart_debug_clear_buffer(void);

/* Exported functions --------------------------------------------------------*/

/******************************************************************************/
/*!
 *    @brief  Initialises UART reception in interrupt mode.
 *            Must be called after MX_USART1_UART_Init().
 *
 *    @param  None
 *    @retval None
 */
void uart_debug_init(void)
{
    uart_debug_clear_buffer();

    uart_debug_send_string("UART debug ready\r\n");

    HAL_UART_Receive_IT(&huart1, &rx_byte, 1U);
}

/******************************************************************************/
/*!
 *    @brief  UART RX complete callback.
 *            This function is called automatically by the HAL when one byte
 *            has been received.
 *
 *    @param  huart UART handle pointer
 *    @retval None
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        if ((rx_byte == '\r') || (rx_byte == '\n'))
        {
            if (rx_index > 0U)
            {
                rx_buffer[rx_index] = '\0';
                uart_debug_process_command(rx_buffer);
                uart_debug_clear_buffer();
            }
        }
        else
        {
            if (rx_index < (UART_RX_BUFFER_SIZE - 1U))
            {
                rx_buffer[rx_index] = (char)rx_byte;
                rx_index++;
            }
            else
            {
                uart_debug_clear_buffer();
                uart_debug_send_string("Command too long\r\n");
            }
        }

        HAL_UART_Receive_IT(&huart1, &rx_byte, 1U);
    }
}

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*!
 *    @brief  Processes received UART command.
 *
 *    @param  cmd Received command string
 *    @retval None
 */
static void uart_debug_process_command(char *cmd)
{
    if (strcmp(cmd, "redLED_on") == 0)
    {
        LED_RED_ON();
        uart_debug_send_string("redLED : on\r\n");
    }
    else if (strcmp(cmd, "greenLED_on") == 0)
    {
        LED_GREEN_ON();
        uart_debug_send_string("greenLED : on\r\n");
    }
    else if (strcmp(cmd, "yellowLED_on") == 0)
    {
        LED_YELLOW_ON();
        uart_debug_send_string("yellowLED : on\r\n");
    }
    else if (strcmp(cmd, "redLED_off") == 0)
    {
        LED_RED_OFF();
        uart_debug_send_string("redLED : off\r\n");
    }
    else if (strcmp(cmd, "greenLED_off") == 0)
    {
        LED_GREEN_OFF();
        uart_debug_send_string("greenLED : off\r\n");
    }
    else if (strcmp(cmd, "yellowLED_off") == 0)
    {
        LED_YELLOW_OFF();
        uart_debug_send_string("yellowLED : off\r\n");
    }
    else if (strcmp(cmd, "toggleLEDs") == 0)
    {
        LED_RED_TOGGLE();
        LED_GREEN_TOGGLE();
        LED_YELLOW_TOGGLE();
        uart_debug_send_string("LEDs toggled\r\n");
    }
    else if (strcmp(cmd, "readADC") == 0)
    {
        uart_debug_send_string("AnalogSensor Temp value = \r\n");
    }
    else if (strcmp(cmd, "readSPI_TMP") == 0)
    {
        uart_debug_send_string("DigitalSensor Temp value = \r\n");
    }
    else if (strcmp(cmd, "readI2C") == 0)
    {
        uart_debug_send_string("TBD = \r\n");
    }
    else
    {
        uart_debug_send_string("Unknown command\r\n");
    }
}

/******************************************************************************/
/*!
 *    @brief  Sends a string through USART1.
 *
 *    @param  msg Pointer to string
 *    @retval None
 */
static void uart_debug_send_string(const char *msg)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

/******************************************************************************/
/*!
 *    @brief  Clears RX command buffer.
 *
 *    @param  None
 *    @retval None
 */
static void uart_debug_clear_buffer(void)
{
    memset(rx_buffer, 0, UART_RX_BUFFER_SIZE);
    rx_index = 0U;
}

/****** END OF FILE ***********************************************************/
