/*************************************************************************//**
 *
 *    @file           analog_temp.c
 *    @date           12.05.2026
 *    @version        0.1.0
 *
 *    @author         Samuele Masciadri
 *
 ******************************************************************************
 *
 *    @brief          Analog temperature sensor driver using ADC.
 *
 *                    The analog temperature sensor is an LM235.
 *                    Its output is proportional to absolute temperature:
 *                    10 mV / Kelvin.
 *
 ******************************************************************************/

/* Private includes ----------------------------------------------------------*/
#include "analog_temp.h"

/* Private defines -----------------------------------------------------------*/

#define ANALOG_TEMP_ADC_MAX_VALUE       4095U
#define ANALOG_TEMP_VREF_MV             3439U

#define KELVIN_OFFSET_CENTI             27315L

/* Private data types --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

static ADC_HandleTypeDef *analog_temp_hadc = NULL;

/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/******************************************************************************/
/*!
 *    @brief  Initialises analog temperature driver.
 *
 *    @param  hadc ADC handle pointer
 *    @retval None
 */
void analog_temp_init(ADC_HandleTypeDef *hadc)
{
    analog_temp_hadc = hadc;

    if (analog_temp_hadc != NULL)
    {
        HAL_ADCEx_Calibration_Start(analog_temp_hadc);
    }
}

/******************************************************************************/
/*!
 *    @brief  Reads raw ADC value.
 *
 *    @param  None
 *    @retval Raw ADC value
 */
uint32_t analog_temp_read_raw(void)
{
    uint32_t value = 0U;

    if (analog_temp_hadc == NULL)
    {
        return 0U;
    }

    if (HAL_ADC_Start(analog_temp_hadc) != HAL_OK)
    {
        return 0U;
    }

    if (HAL_ADC_PollForConversion(analog_temp_hadc, 10U) == HAL_OK)
    {
        value = HAL_ADC_GetValue(analog_temp_hadc);
    }

    HAL_ADC_Stop(analog_temp_hadc);

    return value;
}

/******************************************************************************/
/*!
 *    @brief  Converts raw ADC value to millivolts.
 *
 *    @param  raw Raw ADC value
 *    @retval Voltage in millivolts
 */
uint32_t analog_temp_raw_to_millivolts(uint32_t raw)
{
    return (raw * ANALOG_TEMP_VREF_MV) / ANALOG_TEMP_ADC_MAX_VALUE;
}

/******************************************************************************/
/*!
 *    @brief  Converts LM235 voltage to centi-Celsius.
 *
 *    @param  voltage_mv Sensor voltage in millivolts
 *    @retval Temperature in centi-Celsius
 */
int32_t analog_temp_millivolts_to_centi_celsius(uint32_t voltage_mv)
{
    /*
     * LM235:
     * T[K] = V[mV] / 10
     *
     * T[cC] = T[K] * 100 - 27315
     * T[cC] = V[mV] * 10 - 27315
     */

    return (((int32_t)voltage_mv) * 10L) - KELVIN_OFFSET_CENTI;
}

/******************************************************************************/
/*!
 *    @brief  Reads ADC value and converts it to voltage.
 *
 *    @param  None
 *    @retval Voltage in volts
 */
float analog_temp_read_voltage(void)
{
    uint32_t raw;
    uint32_t voltage_mv;

    raw = analog_temp_read_raw();
    voltage_mv = analog_temp_raw_to_millivolts(raw);

    return ((float)voltage_mv) / 1000.0f;
}

/******************************************************************************/
/*!
 *    @brief  Reads LM235 temperature in Celsius.
 *
 *    @param  None
 *    @retval Temperature in Celsius
 */
float analog_temp_read_lm235_celsius(void)
{
    uint32_t raw;
    uint32_t voltage_mv;
    int32_t temp_centi;

    raw = analog_temp_read_raw();
    voltage_mv = analog_temp_raw_to_millivolts(raw);
    temp_centi = analog_temp_millivolts_to_centi_celsius(voltage_mv);

    return ((float)temp_centi) / 100.0f;
}

/****** END OF FILE ***********************************************************/

