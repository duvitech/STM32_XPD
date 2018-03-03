/**
  ******************************************************************************
  * @file    xpd_pwr.h
  * @author  Benedek Kupper
  * @version 0.2
  * @date    2018-01-28
  * @brief   STM32 eXtensible Peripheral Drivers Power Module
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
#ifndef __XPD_PWR_H_
#define __XPD_PWR_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <xpd_common.h>

/** @defgroup PWR
 * @{ */

/** @defgroup PWR_Core PWR Core
 * @{ */

/** @defgroup PWR_Exported_Types PWR Exported Types
 * @{ */

/** @brief PWR regulator types */
typedef enum
{
    PWR_MAINREGULATOR     = 0, /*!< Main regulator ON in Sleep/Stop mode */
    PWR_LOWPOWERREGULATOR = 1, /*!< Low Power regulator ON in Sleep/Stop mode */
#ifdef PWR_CR_UDEN
    PWR_MAINREGULATOR_UNDERDRIVE_ON     = PWR_CR_MRUDS,     /*!< Main regulator ON in Underdrive mode */
    PWR_LOWPOWERREGULATOR_UNDERDRIVE_ON = PWR_CR_LPUDS | 1, /*!< Low Power regulator ON in Underdrive mode */
#endif
}PWR_RegulatorType;

/** @} */

/** @defgroup PWR_Exported_Macros PWR Exported Macros
 * @{ */

/**
 * @brief  Get the specified PWR flag.
 * @param  FLAG_NAME: specifies the flag to return.
 *         This parameter can be one of the following values:
 *            @arg WUF:         Wake up flag
 *            @arg SBF:         Standby flag
 *            @arg PVDO:        Power Voltage Detector output flag
 *            @arg BRR:         Backup regulator ready flag
 *            @arg VOSRDY:      Regulator voltage scaling output selection ready flag
 */
#define         PWR_FLAG_STATUS(FLAG_NAME)      \
    (PWR_REG_BIT(CSR,FLAG_NAME))

/**
 * @brief  Clear the specified PWR flag.
 * @param  FLAG_NAME: specifies the flag to clear.
 *         This parameter can be one of the following values:
 *            @arg WUF:         Wake up flag
 *            @arg SBF:         Standby flag
 */
#define         PWR_FLAG_CLEAR(FLAG_NAME)       \
    (PWR_REG_BIT(CR,C##FLAG_NAME) = 1)

#ifdef PWR_BB
/**
 * @brief PWR register bit accessing macro
 * @param REG_NAME: specifies the register name.
 * @param BIT_NAME: specifies the register bit name.
 */
#define         PWR_REG_BIT(REG_NAME, BIT_NAME) \
    (PWR_BB->REG_NAME.BIT_NAME)
#else
/**
 * @brief PWR register bit accessing macro
 * @param REG_NAME: specifies the register name.
 * @param BIT_NAME: specifies the register bit name.
 */
#define         PWR_REG_BIT(REG_NAME, BIT_NAME) \
    (PWR->REG_NAME.b.BIT_NAME)
#endif

/** @} */

/** @addtogroup PWR_Exported_Functions
 * @{ */
void            PWR_vSleepMode           (ReactionType eWakeUpOn);
void            PWR_vStopMode            (ReactionType eWakeUpOn, PWR_RegulatorType eRegulator);
void            PWR_vStandbyMode         (void);

XPD_ReturnType  PWR_eBackupRegulatorCtrl (FunctionalState eNewState);

void            PWR_vWakeUpPin_Enable    (uint8_t ucWakeUpPin);
void            PWR_vWakeUpPin_Disable   (uint8_t ucWakeUpPin);
#ifdef PWR_CSR_WUPP
void            PWR_vWakeUpPin_Polarity  (uint8_t ucWakeUpPin, EdgeType eRisingOrFalling);
#endif

/**
 * @brief Send Event on Pending bit enables disabled interrupts to wake up
 *        a system from WaitForEvent.
 * @param eNewState: the new SEVONPEND value to set
 */
__STATIC_INLINE void PWR_vSEVONPEND(FunctionalState eNewState)
{
    SCB->SCR.b.SEVONPEND = eNewState;
}

/**
 * @brief Sleep on Exit bit enables to enter sleep mode
 *        on return from an ISR to Thread mode.
 * @param eNewState: the new SLEEPONEXIT value to set
 */
__STATIC_INLINE void PWR_vSLEEPONEXIT(FunctionalState eNewState)
{
    SCB->SCR.b.SLEEPONEXIT = eNewState;
}

/**
 * @brief Sleep Deep bit enables to enter deep sleep mode.
 * @param eNewState: the new SLEEPONEXIT value to set
 */
__STATIC_INLINE void PWR_vSLEEPDEEP(FunctionalState eNewState)
{
    SCB->SCR.b.SLEEPDEEP = eNewState;
}

/** @} */

/** @} */

/** @defgroup PWR_Peripherals PWR Peripherals
 * @{ */

/** @defgroup PWR_Peripherals_Exported_Functions PWR Peripherals Exported Functions
 * @{ */

/**
 * @brief Enables or disables access to the backup domain
 *        (RTC registers, RTC backup data registers when present).
 * @param eNewState: the new backup access state to set
 * @note  If the HSE divided by 32 is used as the RTC clock, the
 *         Backup Domain Access should be kept enabled.
 */
__STATIC_INLINE void PWR_vBackupAccess(FunctionalState eNewState)
{
    PWR_REG_BIT(CR,DBP) = eNewState;
}

#ifdef PWR_CR_FPDS
/**
 * @brief Sets the Flash Power Down state in Stop mode.
 * @param eNewState: the new Flash power down state to set
 */
__STATIC_INLINE void PWR_vFlashPowerDown(FunctionalState eNewState)
{
    PWR_REG_BIT(CR, FPDS) = eNewState;
}
#endif /* PWR_CR_FPDS */

/** @} */

/** @} */

#ifdef PWR_CR_VOS
/** @defgroup PWR_Regulator_Voltage_Scaling PWR Regulator Voltage Scaling
 * @{ */

/** @defgroup PWR_Regulator_Voltage_Scaling_Exported_Types PWR Regulator Voltage Scaling Exported Types
 * @{ */

/** @brief Regulator Voltage Scaling modes */
typedef enum
{
#ifdef PWR_CR_VOS_1
    PWR_REGVOLT_SCALE1 = 3, /*!< Scale 1 mode(default value at reset): the maximum value of fHCLK is 168 MHz. It can be extended to
                                 180 MHz by activating the over-drive mode. */
    PWR_REGVOLT_SCALE2 = 2, /*!< Scale 2 mode: the maximum value of fHCLK is 144 MHz. It can be extended to
                                 168 MHz by activating the over-drive mode. */
    PWR_REGVOLT_SCALE3 = 1  /*!< Scale 3 mode: the maximum value of fHCLK is 120 MHz. */
#else
    PWR_REGVOLT_SCALE1 = 1, /*!< Scale 1 mode(default value at reset): the maximum value of fHCLK = 168 MHz. */
    PWR_REGVOLT_SCALE2 = 0  /*!< Scale 2 mode: the maximum value of fHCLK = 144 MHz. */
#endif
}PWR_RegVoltScaleType;

/** @} */

/** @addtogroup PWR_Regulator_Voltage_Scaling_Exported_Functions
 * @{ */
XPD_ReturnType  PWR_eVoltageScaleConfig     (PWR_RegVoltScaleType eScaling);
#if defined(PWR_CR_MRLVDS) && defined(PWR_CR_LPLVDS)
void            PWR_vRegLowVoltageConfig    (PWR_RegulatorType eRegulator, FunctionalState eNewState);
#endif

/**
 * @brief Returns the current Regulator Voltage Scaling configuration.
 * @return The active voltage scaling
 */
__STATIC_INLINE PWR_RegVoltScaleType PWR_eGetVoltageScale(void)
{
    return PWR->CR.b.VOS;
}

/** @} */

/** @} */
#endif /* PWR_CR_VOS */

#ifdef PWR_CR_ODEN
/** @defgroup PWR_OverDrive_Mode PWR OverDrive Mode
 * @{ */

/** @addtogroup PWR_OverDrive_Mode_Exported_Functions
 * @{ */
XPD_ReturnType  PWR_eOverDriveEnable        (void);
XPD_ReturnType  PWR_eOverDriveDisable       (void);
/** @} */

/** @} */
#endif /* PWR_CR_ODEN */

/** @} */

#ifdef PWR_CR_PLS
#include <xpd_pvd.h>
#endif /* PWR_CR_PLS */

#ifdef __cplusplus
}
#endif

#endif /* __XPD_PWR_H_ */
