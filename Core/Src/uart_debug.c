/*************************************************************************//**
 *
 *    @file           uart_debug.c
 *    @date           12.05.2026
 *    @version        0.2.0
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
 *                      - help
 *                      - redLED_on
 *                      - greenLED_on
 *                      - yellowLED_on
 *                      - redLED_off
 *                      - greenLED_off
 *                      - yellowLED_off
 *                      - toggleLEDs
 *                      - readADC
 *                      - readSPI_TMP
 *                      - readTMP_ID
 *                      - readI2C
 *
 ******************************************************************************/

/* Private includes ----------------------------------------------------------*/
#include "uart_debug.h"
#include "gpio_debug.h"
#include "tmp126.h"
#include <string.h>
#include <stdio.h>

/* Private defines -----------------------------------------------------------*/

#define UART_RX_BUFFER_SIZE     64U

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
static void uart_debug_format_tmp126_temperature(char *msg, uint32_t msg_size);
static void uart_debug_format_centi_temperature(char *dst,
                                                uint32_t dst_size,
                                                int32_t temp_centi);

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

    uart_debug_send_string("\r\nUART debug ready\r\n");
    uart_debug_send_string("Type help for command list\r\n");

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
        uart_debug_send_string("AnalogSensor Temp value = TBD\r\n");
    }
    else if (strcmp(cmd, "readSPI_TMP") == 0)
    {
        char msg[96];

        uart_debug_format_tmp126_temperature(msg, sizeof(msg));
        uart_debug_send_string(msg);
    }
    else if (strcmp(cmd, "readTMP_ID") == 0)
    {
        char msg[64];
        uint16_t device_id;

        device_id = tmp126_read_device_id();

        snprintf(msg,
                 sizeof(msg),
                 "TMP126 Device ID = 0x%04X\r\n",
                 device_id);

        uart_debug_send_string(msg);
    }
    else if (strcmp(cmd, "readI2C") == 0)
    {
        uart_debug_send_string("TBD = \r\n");
    }
    else if (strcmp(cmd, "help") == 0)
    {
        uart_debug_send_string(
            "Commands:\r\n"
            "  help\r\n"
            "  redLED_on\r\n"
            "  greenLED_on\r\n"
            "  yellowLED_on\r\n"
            "  redLED_off\r\n"
            "  greenLED_off\r\n"
            "  yellowLED_off\r\n"
            "  toggleLEDs\r\n"
            "  readADC\r\n"
            "  readSPI_TMP\r\n"
            "  readTMP_ID\r\n"
            "  readI2C\r\n"
        );
    }
    else
    {
        uart_debug_send_string("Unknown command\r\n");
    }
}

/******************************************************************************/
/*!
 *    @brief  Formats TMP126 raw value and temperature.
 *
 *    @param  msg Destination string
 *    @param  msg_size Destination string size
 *    @retval None
 */
static void uart_debug_format_tmp126_temperature(char *msg, uint32_t msg_size)
{
    uint16_t raw;
    int32_t temp_centi;
    char temp_string[16];

    raw = tmp126_read_raw_temperature();
    temp_centi = tmp126_raw_to_centi_celsius(raw);

    uart_debug_format_centi_temperature(temp_string,
                                        sizeof(temp_string),
                                        temp_centi);

    snprintf(msg,
             msg_size,
             "TMP126 raw=0x%04X, temp=%s C\r\n",
             raw,
             temp_string);
}

/******************************************************************************/
/*!
 *    @brief  Formats a centi-Celsius temperature as signed decimal string.
 *
 *    @param  dst Destination string
 *    @param  dst_size Destination string size
 *    @param  temp_centi Temperature in centi-Celsius
 *    @retval None
 */
static void uart_debug_format_centi_temperature(char *dst,
                                                uint32_t dst_size,
                                                int32_t temp_centi)
{
    char sign = '+';
    uint32_t abs_temp;

    if (temp_centi < 0)
    {
        sign = '-';
        abs_temp = (uint32_t)(-temp_centi);
    }
    else
    {
        abs_temp = (uint32_t)temp_centi;
    }

    snprintf(dst,
             dst_size,
             "%c%lu.%02lu",
             sign,
             abs_temp / 100U,
             abs_temp % 100U);
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
