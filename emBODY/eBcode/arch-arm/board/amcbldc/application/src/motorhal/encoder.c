/*******************************************************************************************************************//**
 * @file    encoder.c
 * @author  G.Zini
 * @version 1.0
 * @date    2021 March, 9
 * @brief   Incremental Encoder management
 **********************************************************************************************************************/
 
// CODE SHAPER
#if defined(USE_STM32HAL)
#include "motorhal_config.h"
#endif
 
 #include "embot_core.h"
 
#if defined(USE_STM32HAL) 
// API
#include "encoder.h"
#endif

/* Includes ***********************************************************************************************************/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

#if defined(USE_STM32HAL)
#include "stm32hal.h"
#if !defined(HALCONFIG_DONTUSE_FLASH)
#include "flash.h"
#endif
#else
#include "FreeRTOS.h"
#include "encoder.h"
#include "flash.h"
#include "main.h"
#include "utilities.h"
#include "tim.h"
#include "stm32g4xx_hal_tim.h"
#endif

#if (USE_HAL_TIM_REGISTER_CALLBACKS != 1)
    #error Flag TIM2 in menu "Project Manager -> Advanced Settings -> Register CallBack" of CubeMx must be ENABLED
#endif

/* Private macros *****************************************************************************************************/
/* Private variables **************************************************************************************************/

/* Coversion factor from encoder step value to elctrical angle. It is given by:
 * encoderConvFactor = 65536 * number_of_poles / number_of_encoder_steps
 */
static volatile int16_t encoderConvFactor = 0;
static volatile uint16_t electricalOffset = 0;
static volatile bool encoderCalibrated = false;
static volatile uint16_t encoderForcedValue = 0;


/* Callbacks **********************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief
 * @param
 * @return
 */
void encoderIndexCallback(TIM_HandleTypeDef *htim)
{
    //++encoderIndex;
    embot::core::print("Index!!!");
}


/* Exported functions *************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief   Start TIM2 as 32 bits incremental encoders
 * @param   void
 * @return  HAL_StatusTypeDef   Operation result
 */
HAL_StatusTypeDef encoderInit(void)
{
    /* here settings not managed by yarprobotinterface but still configurable */
    MainConf.encoder.mode   = TIM_ENCODERMODE_TI12;
    MainConf.encoder.filter = 4;
    MainConf.encoder.idxpos = TIM_ENCODERINDEX_POSITION_00;
    
    /* default values */
    MainConf.encoder.resolution = 0;
    MainConf.encoder.has_hall_sens = 1;
    
    return HAL_OK;
}

HAL_StatusTypeDef encoderDeinit(void)
{
    HAL_TIM_Encoder_Stop(&htim2, TIM_CHANNEL_ALL);
    
    /* default values */
    MainConf.encoder.resolution = 0;
    MainConf.encoder.has_hall_sens = 1;
    
    /* deinitialize */
    encoderConvFactor  = 0;
    electricalOffset   = 0;
    encoderCalibrated  = false;
    encoderForcedValue = 0;
    
    return HAL_OK;
}

HAL_StatusTypeDef encoderConfig(int16_t resolution, uint8_t num_polar_couples, uint8_t has_hall_sens)
{
    TIM_Encoder_InitTypeDef sConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIMEx_EncoderIndexConfigTypeDef sEncoderIndexConfig = {0};
    
    MainConf.encoder.has_hall_sens = has_hall_sens;
    MainConf.pwm.num_polar_couples = num_polar_couples;
    
    if (resolution < 0)
    {
        resolution = -resolution;
        htim2.Init.CounterMode = TIM_COUNTERMODE_DOWN;
    }
    else
    {
        htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    }
    
    MainConf.encoder.resolution = resolution;
    
    if (resolution == 0)
    {
        return HAL_OK;
    }

    /* Forced, for now */
    encoderConvFactor = 65536L*num_polar_couples/resolution;   
    
    /* Re-configure TIM2 base, IC1 and IC2 */
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 0;
    //htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = (resolution/num_polar_couples + 0.5) - 1;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    sConfig.EncoderMode = MainConf.encoder.mode;
    sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC1Filter = MainConf.encoder.filter;
    sConfig.IC2Polarity = /*TIM_ICPOLARITY_FALLING;*/TIM_ICPOLARITY_RISING;
    sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC2Filter = MainConf.encoder.filter;
    if (HAL_OK != HAL_TIM_Encoder_Init(&htim2, &sConfig)) return HAL_ERROR;

    /* Force master mode witout slaves */
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_OK != HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig)) return HAL_ERROR;

    if (MainConf.encoder.has_hall_sens)
    {
        /* no INDEX needed */
        HAL_TIMEx_DisableEncoderIndex(&htim2);
    }
    else
    {
        /* Configure the INDEX mode */
        
        sEncoderIndexConfig.Polarity = TIM_ENCODERINDEX_POLARITY_NONINVERTED;
        sEncoderIndexConfig.Prescaler = TIM_ENCODERINDEX_PRESCALER_DIV1;
        sEncoderIndexConfig.Filter = MainConf.encoder.filter;
        sEncoderIndexConfig.FirstIndexEnable = DISABLE;
        sEncoderIndexConfig.Position = MainConf.encoder.idxpos;
        sEncoderIndexConfig.Direction = TIM_ENCODERINDEX_DIRECTION_UP_DOWN;
        if (HAL_OK != HAL_TIMEx_ConfigEncoderIndex(&htim2, &sEncoderIndexConfig)) return HAL_ERROR;
    
        /* Register the callback function used to signal the activation of the Index pulse */
        if (HAL_OK != HAL_TIM_RegisterCallback(&htim2, HAL_TIM_ENCODER_INDEX_CB_ID, encoderIndexCallback)) return HAL_ERROR;
    }
    
    /* Start timers in encoder mode */
    if (HAL_OK != HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL)) return HAL_ERROR;
    
    __HAL_TIM_SET_COUNTER(&htim2, 0);
  
    return HAL_OK;
}


/*******************************************************************************************************************//**
 * @brief   Read encoder value
 * @param   void
 * @return  int32_t     Encoder value
 */
uint32_t encoderGetCounter(void)
{
    return (uint32_t)__HAL_TIM_GET_COUNTER(&htim2);
}


void encoderReset()
{
    __HAL_TIM_SET_COUNTER(&htim2, 0);
}

/*******************************************************************************************************************//**
 * @brief   Read encoder value and convert to electrical-angle
 * @param   void
 * @return  uint16_t    Encoder value
 */
uint16_t encoderGetElectricalAngle(void)
{
    if (MainConf.encoder.resolution == 0)
    {
        return encoderForcedValue;
    }
    
    if (MainConf.encoder.has_hall_sens)
    {
        if (!encoderCalibrated)
        {
            return encoderForcedValue;
        }
    }
    
    return electricalOffset + (__HAL_TIM_GET_COUNTER(&htim2) * encoderConvFactor) & 0xFFFF;
}

uint16_t encoderGetUncalibrated(void)
{
    return (__HAL_TIM_GET_COUNTER(&htim2) * encoderConvFactor) & 0xFFFF;
}

/*******************************************************************************************************************//**
 * @brief   Set encoder electrical-angle offset
 * @param   uint16_t    Encoder offset
 * @return  void
 */

void encoderForce(uint16_t value)
{
    encoderForcedValue = value;
}

void encoderCalibrate(uint16_t offset)
{   
    electricalOffset = offset;
    
    encoderCalibrated = true;
}

uint16_t encoderGetElectricalOffset()
{
    return electricalOffset;
}

/* END OF FILE ********************************************************************************************************/
