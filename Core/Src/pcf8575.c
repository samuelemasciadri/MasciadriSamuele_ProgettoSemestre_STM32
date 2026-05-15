/*************************************************************************//**
 *
 *    @file           pcf8575.c
 *    @date           15.05.2026
 *    @version        0.1.0
 *
 *    @author         Samuele Masciadri
 *
 ******************************************************************************
 *
 *    @brief          PCF8575 I2C GPIO expander driver.
 *
 ******************************************************************************/

/* Private includes ----------------------------------------------------------*/
#include "pcf8575.h"
#include "sw_i2c.h"

/* Private defines -----------------------------------------------------------*/

/* Private data types --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

static uint8_t pcf8575_address = PCF8575_DEFAULT_ADDRESS;
static uint16_t pcf8575_output_state = 0xFFFFU;

/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/******************************************************************************/
/*!
 *    @brief  Initialises PCF8575 driver.
 *
 *    @param  address_7bit I2C 7-bit address
 *    @retval None
 */
void pcf8575_init(uint8_t address_7bit)
{
    pcf8575_address = address_7bit;
    pcf8575_output_state = 0xFFFFU;

    sw_i2c_init();

    pcf8575_write(pcf8575_output_state);
}

/******************************************************************************/
/*!
 *    @brief  Scans PCF8575 address range.
 *
 *    @param  found_address Pointer to found address variable
 *    @retval 1 if found, 0 otherwise
 */
uint8_t pcf8575_scan(uint8_t *found_address)
{
    uint8_t address;

    for (address = 0x20U; address <= 0x27U; address++)
    {
        if (sw_i2c_probe(address) != 0U)
        {
            if (found_address != NULL)
            {
                *found_address = address;
            }

            pcf8575_address = address;

            return 1U;
        }
    }

    return 0U;
}

/******************************************************************************/
/*!
 *    @brief  Writes 16-bit value to PCF8575 outputs.
 *
 *    @param  value Output value
 *    @retval 1 if successful, 0 otherwise
 */
uint8_t pcf8575_write(uint16_t value)
{
    uint8_t data[2];

    data[0] = (uint8_t)(value & 0x00FFU);
    data[1] = (uint8_t)((value >> 8U) & 0x00FFU);

    if (sw_i2c_write(pcf8575_address, data, 2U) != 0U)
    {
        pcf8575_output_state = value;
        return 1U;
    }

    return 0U;
}

/******************************************************************************/
/*!
 *    @brief  Reads 16-bit value from PCF8575 pins.
 *
 *    @param  value Pointer to read value
 *    @retval 1 if successful, 0 otherwise
 */
uint8_t pcf8575_read(uint16_t *value)
{
    uint8_t data[2];

    if (value == NULL)
    {
        return 0U;
    }

    if (sw_i2c_read(pcf8575_address, data, 2U) == 0U)
    {
        return 0U;
    }

    *value = ((uint16_t)data[1] << 8U) | ((uint16_t)data[0]);

    return 1U;
}

/******************************************************************************/
/*!
 *    @brief  Toggles PCF8575 output state.
 *
 *    @param  None
 *    @retval 1 if successful, 0 otherwise
 */
uint8_t pcf8575_toggle(void)
{
    pcf8575_output_state ^= 0xFFFFU;

    return pcf8575_write(pcf8575_output_state);
}

/******************************************************************************/
/*!
 *    @brief  Reads PCF8575 INT pin connected to MCU.
 *
 *    @param  None
 *    @retval 1 if high, 0 if low
 */
uint8_t pcf8575_read_int_pin(void)
{
    if (HAL_GPIO_ReadPin(PCF8575_INT_GPIO_Port, PCF8575_INT_Pin) == GPIO_PIN_SET)
    {
        return 1U;
    }

    return 0U;
}

/****** END OF FILE ***********************************************************/
