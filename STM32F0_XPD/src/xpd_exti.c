/**
  ******************************************************************************
  * @file    xpd_exti.c
  * @author  Benedek Kupper
  * @version 0.2
  * @date    2018-01-28
  * @brief   STM32 eXtensible Peripheral Drivers EXTI Module
  *
  * Copyright (c) 2018 Benedek Kupper
  *
  * Licensed under the Apache License, Version 2.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  *     http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  */

#include <xpd_exti.h>

XPD_ValueCallbackType EXTI_xPinCallbacks[16] = {
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

/** @addtogroup EXTI
 * @{ */

/** @defgroup EXTI_Exported_Functions EXTI Exported Functions
 * @{ */

/**
 * @brief Configures the EXTI line according to the setup parameters.
 * @param ucLine: the selected EXTI line
 * @param pxConfig: pointer to the setup structure
 */
void EXTI_vInit(uint8_t ucLine, const EXTI_InitType * pxConfig)
{
#ifdef RCC_APB2ENR_EXTITEN
    RCC_vClockEnable(RCC_POS_EXTI);
#endif

#ifdef EXTI_BB
    EXTI_BB->IMR[ucLine] = pxConfig->Reaction;

    EXTI_BB->EMR[ucLine] = pxConfig->Reaction >> 1;

    EXTI_BB->RTSR[ucLine] = pxConfig->Edge;

    EXTI_BB->FTSR[ucLine] = pxConfig->Edge >> 1;
#else
    uint32_t ulLineBit = 1 << ucLine;

    /* set EXTI line configuration */
    if (pxConfig->Reaction & REACTION_IT)
    {
        SET_BIT(EXTI->IMR, ulLineBit);
    }
    else
    {
        CLEAR_BIT(EXTI->IMR, ulLineBit);
    }

    if (pxConfig->Reaction & REACTION_EVENT)
    {
        SET_BIT(EXTI->EMR, ulLineBit);
    }
    else
    {
        CLEAR_BIT(EXTI->EMR, ulLineBit);
    }

    /* set rising and falling edge configuration */
    if (pxConfig->Edge & EDGE_RISING)
    {
        SET_BIT(EXTI->RTSR, ulLineBit);
    }
    else
    {
        CLEAR_BIT(EXTI->RTSR, ulLineBit);
    }

    if (pxConfig->Edge & EDGE_FALLING)
    {
        SET_BIT(EXTI->FTSR, ulLineBit);
    }
    else
    {
        CLEAR_BIT(EXTI->FTSR, ulLineBit);
    }
#endif
}

/**
 * @brief Restores the EXTI line to its default state.
 * @param ucLine: the selected EXTI line
 */
void EXTI_vDeinit(uint8_t ucLine)
{
#ifdef EXTI_BB
    /* Clear EXTI line configuration */
    EXTI_BB->IMR[ucLine] = 0;
    EXTI_BB->EMR[ucLine] = 0;

    /* Clear Rising Falling edge configuration */
    EXTI_BB->RTSR[ucLine] = 0;
    EXTI_BB->FTSR[ucLine] = 0;
#else
    uint32_t ulLineBit = 1 << ucLine;

    /* Clear EXTI line configuration */
    CLEAR_BIT(EXTI->IMR, ulLineBit);
    CLEAR_BIT(EXTI->EMR, ulLineBit);

    /* Clear Rising Falling edge configuration */
    CLEAR_BIT(EXTI->RTSR, ulLineBit);
    CLEAR_BIT(EXTI->FTSR, ulLineBit);
#endif
}

/** @} */

/** @} */
