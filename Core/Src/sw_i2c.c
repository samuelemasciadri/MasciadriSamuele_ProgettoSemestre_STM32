/*************************************************************************//**
 *
 *    @file           sw_i2c.c
 *    @date           15.05.2026
 *    @version        0.1.0
 *
 *    @author         Samuele Masciadri
 *
 ******************************************************************************
 *
 *    @brief          Software I2C driver using GPIO bit-banging.
 *
 ******************************************************************************/

/* Private includes ----------------------------------------------------------*/
#include "sw_i2c.h"

/* Private defines -----------------------------------------------------------*/

#define SW_I2C_DELAY_CYCLES     60U

/* Private data types --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static void sw_i2c_delay(void);

static void sw_i2c_sda_high(void);
static void sw_i2c_sda_low(void);
static void sw_i2c_scl_high(void);
static void sw_i2c_scl_low(void);

static GPIO_PinState sw_i2c_read_sda(void);

static void sw_i2c_start(void);
static void sw_i2c_stop(void);

static uint8_t sw_i2c_write_byte(uint8_t byte);
static uint8_t sw_i2c_read_byte(uint8_t ack);

/* Exported functions --------------------------------------------------------*/

/******************************************************************************/
/*!
 *    @brief  Initialises software I2C lines.
 *
 *    @param  None
 *    @retval None
 */
void sw_i2c_init(void)
{
    HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_SET);
}

/******************************************************************************/
/*!
 *    @brief  Checks if an I2C device answers to a 7-bit address.
 *
 *    @param  address_7bit I2C 7-bit address
 *    @retval 1 if device ACKs, 0 otherwise
 */
uint8_t sw_i2c_probe(uint8_t address_7bit)
{
    uint8_t ack;

    sw_i2c_start();
    ack = sw_i2c_write_byte((uint8_t)(address_7bit << 1U));
    sw_i2c_stop();

    return ack;
}

/******************************************************************************/
/*!
 *    @brief  Writes bytes to an I2C device.
 *
 *    @param  address_7bit I2C 7-bit address
 *    @param  data Data buffer pointer
 *    @param  size Number of bytes to write
 *    @retval 1 if successful, 0 otherwise
 */
uint8_t sw_i2c_write(uint8_t address_7bit, uint8_t *data, uint16_t size)
{
    uint16_t i;

    sw_i2c_start();

    if (sw_i2c_write_byte((uint8_t)(address_7bit << 1U)) == 0U)
    {
        sw_i2c_stop();
        return 0U;
    }

    for (i = 0U; i < size; i++)
    {
        if (sw_i2c_write_byte(data[i]) == 0U)
        {
            sw_i2c_stop();
            return 0U;
        }
    }

    sw_i2c_stop();

    return 1U;
}

/******************************************************************************/
/*!
 *    @brief  Reads bytes from an I2C device.
 *
 *    @param  address_7bit I2C 7-bit address
 *    @param  data Data buffer pointer
 *    @param  size Number of bytes to read
 *    @retval 1 if successful, 0 otherwise
 */
uint8_t sw_i2c_read(uint8_t address_7bit, uint8_t *data, uint16_t size)
{
    uint16_t i;

    if ((data == NULL) || (size == 0U))
    {
        return 0U;
    }

    sw_i2c_start();

    if (sw_i2c_write_byte((uint8_t)((address_7bit << 1U) | 0x01U)) == 0U)
    {
        sw_i2c_stop();
        return 0U;
    }

    for (i = 0U; i < size; i++)
    {
        if (i < (size - 1U))
        {
            data[i] = sw_i2c_read_byte(1U);
        }
        else
        {
            data[i] = sw_i2c_read_byte(0U);
        }
    }

    sw_i2c_stop();

    return 1U;
}

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*!
 *    @brief  Software I2C timing delay.
 *
 *    @param  None
 *    @retval None
 */
static void sw_i2c_delay(void)
{
    volatile uint32_t i;

    for (i = 0U; i < SW_I2C_DELAY_CYCLES; i++)
    {
        __NOP();
    }
}

/******************************************************************************/
/*!
 *    @brief  Releases SDA line.
 *
 *    @param  None
 *    @retval None
 */
static void sw_i2c_sda_high(void)
{
    HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_SET);
}

/******************************************************************************/
/*!
 *    @brief  Pulls SDA line low.
 *
 *    @param  None
 *    @retval None
 */
static void sw_i2c_sda_low(void)
{
    HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_RESET);
}

/******************************************************************************/
/*!
 *    @brief  Releases SCL line.
 *
 *    @param  None
 *    @retval None
 */
static void sw_i2c_scl_high(void)
{
    HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_SET);
}

/******************************************************************************/
/*!
 *    @brief  Pulls SCL line low.
 *
 *    @param  None
 *    @retval None
 */
static void sw_i2c_scl_low(void)
{
    HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_RESET);
}

/******************************************************************************/
/*!
 *    @brief  Reads SDA line state.
 *
 *    @param  None
 *    @retval SDA line state
 */
static GPIO_PinState sw_i2c_read_sda(void)
{
    return HAL_GPIO_ReadPin(I2C_SDA_GPIO_Port, I2C_SDA_Pin);
}

/******************************************************************************/
/*!
 *    @brief  Generates I2C start condition.
 *
 *    @param  None
 *    @retval None
 */
static void sw_i2c_start(void)
{
    sw_i2c_sda_high();
    sw_i2c_scl_high();
    sw_i2c_delay();

    sw_i2c_sda_low();
    sw_i2c_delay();

    sw_i2c_scl_low();
    sw_i2c_delay();
}

/******************************************************************************/
/*!
 *    @brief  Generates I2C stop condition.
 *
 *    @param  None
 *    @retval None
 */
static void sw_i2c_stop(void)
{
    sw_i2c_sda_low();
    sw_i2c_delay();

    sw_i2c_scl_high();
    sw_i2c_delay();

    sw_i2c_sda_high();
    sw_i2c_delay();
}

/******************************************************************************/
/*!
 *    @brief  Writes one byte on I2C bus.
 *
 *    @param  byte Byte to write
 *    @retval 1 if ACK received, 0 otherwise
 */
static uint8_t sw_i2c_write_byte(uint8_t byte)
{
    uint8_t i;
    uint8_t ack;

    for (i = 0U; i < 8U; i++)
    {
        if ((byte & 0x80U) != 0U)
        {
            sw_i2c_sda_high();
        }
        else
        {
            sw_i2c_sda_low();
        }

        sw_i2c_delay();

        sw_i2c_scl_high();
        sw_i2c_delay();

        sw_i2c_scl_low();
        sw_i2c_delay();

        byte <<= 1U;
    }

    sw_i2c_sda_high();
    sw_i2c_delay();

    sw_i2c_scl_high();
    sw_i2c_delay();

    if (sw_i2c_read_sda() == GPIO_PIN_RESET)
    {
        ack = 1U;
    }
    else
    {
        ack = 0U;
    }

    sw_i2c_scl_low();
    sw_i2c_delay();

    return ack;
}

/******************************************************************************/
/*!
 *    @brief  Reads one byte from I2C bus.
 *
 *    @param  ack 1 to send ACK, 0 to send NACK
 *    @retval Read byte
 */
static uint8_t sw_i2c_read_byte(uint8_t ack)
{
    uint8_t i;
    uint8_t byte = 0U;

    sw_i2c_sda_high();

    for (i = 0U; i < 8U; i++)
    {
        byte <<= 1U;

        sw_i2c_scl_high();
        sw_i2c_delay();

        if (sw_i2c_read_sda() == GPIO_PIN_SET)
        {
            byte |= 0x01U;
        }

        sw_i2c_scl_low();
        sw_i2c_delay();
    }

    if (ack != 0U)
    {
        sw_i2c_sda_low();
    }
    else
    {
        sw_i2c_sda_high();
    }

    sw_i2c_delay();

    sw_i2c_scl_high();
    sw_i2c_delay();

    sw_i2c_scl_low();
    sw_i2c_delay();

    sw_i2c_sda_high();

    return byte;
}

/****** END OF FILE ***********************************************************/
