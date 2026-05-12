/*************************************************************************//**
 *
 *    @file           tmp126.c
 *    @date           12.05.2026
 *    @version        0.1.0
 *
 *    @author         Samuele Masciadri
 *
 ******************************************************************************
 *
 *    @brief          TMP126 digital temperature sensor driver.
 *
 *                    This implementation uses GPIO bit-banging because the
 *                    TMP126 SIO pin is connected to PA6 only.
 *
 ******************************************************************************/

/* Private includes ----------------------------------------------------------*/
#include "tmp126.h"

/* Private defines -----------------------------------------------------------*/

#define TMP126_CMD_READ             0x0100U

#define TMP126_REG_TEMPERATURE      0x00U
#define TMP126_REG_DEVICE_ID        0x0CU

#define TMP126_DELAY_CYCLES         20U

/* Private data types --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static void tmp126_delay(void);

static void tmp126_sio_output(void);
static void tmp126_sio_input(void);

static void tmp126_write_bit(uint8_t bit_value);
static uint8_t tmp126_read_bit(void);

static void tmp126_write_word(uint16_t word);
static uint16_t tmp126_read_word(void);

/* Exported functions --------------------------------------------------------*/

/******************************************************************************/
/*!
 *    @brief  Initialises TMP126 GPIO lines.
 *
 *    @param  None
 *    @retval None
 */
void tmp126_init(void)
{
    HAL_GPIO_WritePin(TMP126_CS_GPIO_Port, TMP126_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(TMP126_SCK_GPIO_Port, TMP126_SCK_Pin, GPIO_PIN_RESET);

    tmp126_sio_input();
}

/******************************************************************************/
/*!
 *    @brief  Reads a 16-bit register from TMP126.
 *
 *    @param  reg_addr Register address
 *    @retval Register value
 */
uint16_t tmp126_read_register(uint8_t reg_addr)
{
    uint16_t command;
    uint16_t data;

    command = TMP126_CMD_READ | ((uint16_t)reg_addr);

    HAL_GPIO_WritePin(TMP126_CS_GPIO_Port, TMP126_CS_Pin, GPIO_PIN_RESET);
    tmp126_delay();

    tmp126_sio_output();
    tmp126_write_word(command);

    tmp126_sio_input();
    data = tmp126_read_word();

    tmp126_delay();
    HAL_GPIO_WritePin(TMP126_CS_GPIO_Port, TMP126_CS_Pin, GPIO_PIN_SET);

    return data;
}

/******************************************************************************/
/*!
 *    @brief  Reads the TMP126 raw temperature register.
 *
 *    @param  None
 *    @retval Raw temperature register value
 */
uint16_t tmp126_read_raw_temperature(void)
{
    return tmp126_read_register(TMP126_REG_TEMPERATURE);
}

/******************************************************************************/
/*!
 *    @brief  Reads the TMP126 device ID register.
 *
 *    @param  None
 *    @retval Device ID register value
 */
uint16_t tmp126_read_device_id(void)
{
    return tmp126_read_register(TMP126_REG_DEVICE_ID);
}

/******************************************************************************/
/*!
 *    @brief  Converts TMP126 raw temperature to centi-Celsius.
 *
 *    @param  raw Raw temperature register value
 *    @retval Temperature in centi-Celsius
 */
int32_t tmp126_raw_to_centi_celsius(uint16_t raw)
{
    int16_t temp14;

    /*
     * TMP126 temperature data is stored in bits [15:2].
     * The value is a 14-bit two's complement number.
     *
     * LSB = 0.03125 Celsius.
     * 0.03125 Celsius = 3.125 centi-Celsius.
     *
     * temp_centi = temp14 * 3125 / 1000
     */

    temp14 = (int16_t)(raw >> 2U);

    if ((temp14 & 0x2000) != 0)
    {
        temp14 |= (int16_t)0xC000;
    }

    return (((int32_t)temp14) * 3125L) / 1000L;
}

/******************************************************************************/
/*!
 *    @brief  Converts TMP126 raw temperature to Celsius.
 *
 *    @param  raw Raw temperature register value
 *    @retval Temperature in Celsius
 */
float tmp126_raw_to_celsius(uint16_t raw)
{
    return ((float)tmp126_raw_to_centi_celsius(raw)) / 100.0f;
}

/******************************************************************************/
/*!
 *    @brief  Reads TMP126 temperature in Celsius.
 *
 *    @param  None
 *    @retval Temperature in Celsius
 */
float tmp126_read_temperature_c(void)
{
    uint16_t raw;

    raw = tmp126_read_raw_temperature();

    return tmp126_raw_to_celsius(raw);
}

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*!
 *    @brief  Small delay for GPIO bit-banging.
 *
 *    @param  None
 *    @retval None
 */
static void tmp126_delay(void)
{
    volatile uint32_t i;

    for (i = 0U; i < TMP126_DELAY_CYCLES; i++)
    {
        __NOP();
    }
}

/******************************************************************************/
/*!
 *    @brief  Configures TMP126 SIO pin as output.
 *
 *    @param  None
 *    @retval None
 */
static void tmp126_sio_output(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = TMP126_SIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(TMP126_SIO_GPIO_Port, &GPIO_InitStruct);
}

/******************************************************************************/
/*!
 *    @brief  Configures TMP126 SIO pin as input.
 *
 *    @param  None
 *    @retval None
 */
static void tmp126_sio_input(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = TMP126_SIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(TMP126_SIO_GPIO_Port, &GPIO_InitStruct);
}

/******************************************************************************/
/*!
 *    @brief  Writes one bit to TMP126 SIO line.
 *
 *    @param  bit_value Bit to write
 *    @retval None
 */
static void tmp126_write_bit(uint8_t bit_value)
{
    if (bit_value != 0U)
    {
        HAL_GPIO_WritePin(TMP126_SIO_GPIO_Port, TMP126_SIO_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(TMP126_SIO_GPIO_Port, TMP126_SIO_Pin, GPIO_PIN_RESET);
    }

    tmp126_delay();

    HAL_GPIO_WritePin(TMP126_SCK_GPIO_Port, TMP126_SCK_Pin, GPIO_PIN_SET);
    tmp126_delay();

    HAL_GPIO_WritePin(TMP126_SCK_GPIO_Port, TMP126_SCK_Pin, GPIO_PIN_RESET);
    tmp126_delay();
}

/******************************************************************************/
/*!
 *    @brief  Reads one bit from TMP126 SIO line.
 *
 *    @param  None
 *    @retval Read bit value
 */
static uint8_t tmp126_read_bit(void)
{
    uint8_t bit_value;

    HAL_GPIO_WritePin(TMP126_SCK_GPIO_Port, TMP126_SCK_Pin, GPIO_PIN_SET);
    tmp126_delay();

    if (HAL_GPIO_ReadPin(TMP126_SIO_GPIO_Port, TMP126_SIO_Pin) == GPIO_PIN_SET)
    {
        bit_value = 1U;
    }
    else
    {
        bit_value = 0U;
    }

    HAL_GPIO_WritePin(TMP126_SCK_GPIO_Port, TMP126_SCK_Pin, GPIO_PIN_RESET);
    tmp126_delay();

    return bit_value;
}

/******************************************************************************/
/*!
 *    @brief  Writes a 16-bit word to TMP126, MSB first.
 *
 *    @param  word Word to write
 *    @retval None
 */
static void tmp126_write_word(uint16_t word)
{
    int8_t bit_index;

    for (bit_index = 15; bit_index >= 0; bit_index--)
    {
        tmp126_write_bit((uint8_t)((word >> bit_index) & 0x0001U));
    }
}

/******************************************************************************/
/*!
 *    @brief  Reads a 16-bit word from TMP126, MSB first.
 *
 *    @param  None
 *    @retval Read word
 */
static uint16_t tmp126_read_word(void)
{
    uint16_t word = 0U;
    uint8_t bit_index;

    for (bit_index = 0U; bit_index < 16U; bit_index++)
    {
        word <<= 1U;
        word |= tmp126_read_bit();
    }

    return word;
}

/****** END OF FILE ***********************************************************/
