/**
  ******************************************************************************
  * @file    xpd_tim.h
  * @author  Benedek Kupper
  * @version V0.1
  * @date    2016-01-26
  * @brief   STM32 eXtensible Peripheral Drivers Timer Module
  *
  *  This file is part of STM32_XPD.
  *
  *  STM32_XPD is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  STM32_XPD is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with STM32_XPD.  If not, see <http://www.gnu.org/licenses/>.
  */
#ifndef XPD_TIM_H_
#define XPD_TIM_H_

#include "xpd_common.h"
#include "xpd_config.h"
#include "xpd_dma.h"

/** @defgroup TIM
 * @{ */

/** @defgroup TIM_Common TIM Common
 * @{ */

/** @defgroup TIM_Common_Exported_Types TIM Common Exported Types
 * @{ */

/** @brief TIM counter modes */
typedef enum
{
    TIM_COUNTER_UP             = 0, /*!< Timer is counting from 0 to Period - 1 */
    TIM_COUNTER_DOWN           = 1, /*!< Timer is counting from Period - 1 to 0 */
    TIM_COUNTER_CENTERALIGNED1 = 2, /*!< Timer is switching between up- and downcounting,
                                         output channel interrupt is set during down */
    TIM_COUNTER_CENTERALIGNED2 = 4, /*!< Timer is switching between up- and downcounting,
                                         output channel interrupt is set during up */
    TIM_COUNTER_CENTERALIGNED3 = 6  /*!< Timer is switching between up- and downcounting,
                                         output channel interrupt is set during up & down */
}TIM_CounterType;

/** @brief TIM counter setup structure */
typedef struct
{
    uint32_t         Prescaler;         /*!< Clock prescaler for the counter, [1 .. (1 << timer bit size)] */
    uint32_t         Period;            /*!< Counter period, [1 .. (1 << timer bit size)] */
    TIM_CounterType  Mode;              /*!< Counter mode */
    ClockDividerType ClockDivision;     /*!< Division factor for deadtime and sampling clock.
                                             Permitted values: @arg CLK_DIV1 @arg CLK_DIV2 @arg CLK_DIV4 */
    uint8_t          RepetitionCounter; /*!< Specifies how many counter periods trigger an update
                                             @note Valid only for TIM1 and TIM8. */
} TIM_CounterInitType;

/** @brief TIM channels */
typedef enum
{
    TIM_CHANNEL_1 = 0, /*!< TIM channel 1 */
    TIM_CHANNEL_2 = 1, /*!< TIM channel 2 */
    TIM_CHANNEL_3 = 2, /*!< TIM channel 3 */
    TIM_CHANNEL_4 = 3  /*!< TIM channel 4 */
}TIM_ChannelType;

/** @brief TIM Handle structure */
typedef struct
{
    TIM_TypeDef * Inst;                     /*!< The address of the peripheral instance used by the handle */
#ifdef TIM_BB
    TIM_BitBand_TypeDef * Inst_BB;          /*!< The address of the peripheral instance in the bit-band region */
#endif
    struct {
        XPD_HandleCallbackType DepInit;      /*!< Callback to initialize module dependencies (GPIOs, IRQs, DMAs) */
        XPD_HandleCallbackType DepDeinit;    /*!< Callback to restore module dependencies (GPIOs, IRQs, DMAs) */
        XPD_HandleCallbackType Update;       /*!< Timer update callback */
        XPD_HandleCallbackType ChannelEvent; /*!< Channel compare/capture callback */
        XPD_HandleCallbackType Trigger;      /*!< Trigger callback */
        XPD_HandleCallbackType Break;        /*!< Break callback */
        XPD_HandleCallbackType Commutation;  /*!< Commutation callback */
        XPD_HandleCallbackType Error;        /*!< DMA error callback */
    }Callbacks;                              /*   Handle Callbacks */
    struct {
        DMA_HandleType * Update;             /*!< DMA handle for update transfer */
        DMA_HandleType * Channel[4];         /*!< DMA handles for channel transfers */
        DMA_HandleType * Commutation;        /*!< DMA handle for commutation transfer */
        DMA_HandleType * Trigger;            /*!< DMA handle for trigger transfer */
    }DMA;                                    /*   DMA handle references */
    volatile TIM_ChannelType ActiveChannel;  /*!< The currently active timer channel */
    volatile uint8_t EnabledChannels;        /*!< The enabled timer channels */
}TIM_HandleType;

/** @} */

/** @addtogroup TIM_Common_Exported_Functions
 * @{ */
XPD_ReturnType  XPD_TIM_Init                (TIM_HandleType * htim, TIM_CounterInitType * Config);
XPD_ReturnType  XPD_TIM_Deinit              (TIM_HandleType * htim);

void            XPD_TIM_CounterStart        (TIM_HandleType * htim);
void            XPD_TIM_CounterStop         (TIM_HandleType * htim);
void            XPD_TIM_CounterStart_IT     (TIM_HandleType * htim);
void            XPD_TIM_CounterStop_IT      (TIM_HandleType * htim);
void            XPD_TIM_CounterStart_DMA    (TIM_HandleType * htim, void * Address, uint16_t Length);
void            XPD_TIM_CounterStop_DMA     (TIM_HandleType * htim);

TIM_CounterType XPD_TIM_CounterDirection    (TIM_HandleType * htim);

uint32_t        XPD_TIM_GetCounter          (TIM_HandleType * htim);
void            XPD_TIM_SetCounter          (TIM_HandleType * htim, uint32_t Value);

TIM_ChannelType XPD_TIM_GetCurrentChannel   (TIM_HandleType * htim);

void            XPD_TIM_ChannelEnable       (TIM_HandleType * htim, TIM_ChannelType Channel);
void            XPD_TIM_ChannelDisable      (TIM_HandleType * htim, TIM_ChannelType Channel);
void            XPD_TIM_ChannelSetPulse     (TIM_HandleType * htim, TIM_ChannelType Channel, uint32_t Pulse);
uint32_t        XPD_TIM_ChannelGetPulse     (TIM_HandleType * htim, TIM_ChannelType Channel);

void            XPD_TIM_CompChannelEnable   (TIM_HandleType * htim, TIM_ChannelType Channel);
void            XPD_TIM_CompChannelDisable  (TIM_HandleType * htim, TIM_ChannelType Channel);

void            XPD_TIM_UP_IRQHandler       (TIM_HandleType * htim);
void            XPD_TIM_CC_IRQHandler       (TIM_HandleType * htim);
void            XPD_TIM_TRG_COM_IRQHandler  (TIM_HandleType * htim);
void            XPD_TIM_BRK_IRQHandler      (TIM_HandleType * htim);
void            XPD_TIM_IRQHandler          (TIM_HandleType * htim);
/** @} */

/** @defgroup TIM_Common_Exported_Macros TIM Common Exported Macros
 * @{ */

/**
 * @brief  TIM Handle initializer macro
 * @param  INSTANCE: specifies the TIM peripheral instance.
 * @param  INIT_FN: specifies the dependency initialization function to call back.
 * @param  DEINIT_FN: specifies the dependency deinitialization function to call back.
 */
#define         NEW_TIM_HANDLE(INSTANCE,INIT_FN,DEINIT_FN)              \
    {.Inst      = (INSTANCE),                                           \
     .Callbacks = {(INIT_FN),(DEINIT_FN),NULL,NULL,NULL,NULL,NULL,NULL}}

/**
 * @brief  Enable the specified TIM interrupt.
 * @param  HANDLE: specifies the TIM Handle.
 * @param  IT_NAME: specifies the interrupt to enable.
 *         This parameter can be one of the following values:
 *            @arg U:       Update
 *            @arg CC1:     Capture/Compare channel 1
 *            @arg CC2:     Capture/Compare channel 2
 *            @arg CC3:     Capture/Compare channel 3
 *            @arg CC4:     Capture/Compare channel 4
 *            @arg COM:     Commutation
 *            @arg T:       Trigger
 *            @arg B:       Break
 */
#define         XPD_TIM_EnableIT(HANDLE, IT_NAME)   \
    (TIM_REG_BIT((HANDLE),DIER,IT_NAME##IE) = 1)

/**
 * @brief  Disable the specified TIM interrupt.
 * @param  HANDLE: specifies the TIM Handle.
 * @param  IT_NAME: specifies the interrupt to enable.
 *         This parameter can be one of the following values:
 *            @arg U:       Update
 *            @arg CC1:     Capture/Compare channel 1
 *            @arg CC2:     Capture/Compare channel 2
 *            @arg CC3:     Capture/Compare channel 3
 *            @arg CC4:     Capture/Compare channel 4
 *            @arg COM:     Commutation
 *            @arg T:       Trigger
 *            @arg B:       Break
 */
#define         XPD_TIM_DisableIT(HANDLE, IT_NAME)  \
    (TIM_REG_BIT((HANDLE),DIER,IT_NAME##IE) = 0)

/**
 * @brief  Get the specified TIM flag.
 * @param  HANDLE: specifies the TIM Handle.
 * @param  FLAG_NAME: specifies the flag to return.
 *         This parameter can be one of the following values:
 *            @arg U:       Update
 *            @arg CC1:     Capture/Compare channel 1
 *            @arg CC2:     Capture/Compare channel 2
 *            @arg CC3:     Capture/Compare channel 3
 *            @arg CC4:     Capture/Compare channel 4
 *            @arg COM:     Commutation
 *            @arg T:       Trigger
 *            @arg B:       Break
 */
#define         XPD_TIM_GetFlag(HANDLE, FLAG_NAME)  \
    (TIM_REG_BIT((HANDLE),SR,FLAG_NAME##IF))

/**
 * @brief  Clear the specified TIM flag.
 * @param  HANDLE: specifies the TIM Handle.
 * @param  FLAG_NAME: specifies the flag to clear.
 *         This parameter can be one of the following values:
 *            @arg U:       Update
 *            @arg CC1:     Capture/Compare channel 1
 *            @arg CC2:     Capture/Compare channel 2
 *            @arg CC3:     Capture/Compare channel 3
 *            @arg CC4:     Capture/Compare channel 4
 *            @arg COM:     Commutation
 *            @arg T:       Trigger
 *            @arg B:       Break
 */
#define         XPD_TIM_ClearFlag(HANDLE,FLAG_NAME) \
    (TIM_REG_BIT((HANDLE),SR,FLAG_NAME##IF) = 0)


#define         XPD_TIM_ChannelGetFlag(HANDLE, CH)  \
    ((HANDLE)->Inst->SR.w & (TIM_SR_CC1IF << (CH)))

#define         XPD_TIM_ChannelClearFlag(HANDLE, CH)\
    SET_BIT((HANDLE)->Inst->SR.w,(TIM_SR_CC1IF << (CH)))

#define         XPD_TIM_ChannelEnableIT(HANDLE, CH) \
    SET_BIT((HANDLE)->Inst->DIER.w,(TIM_DIER_CC1IE << (CH)))

#define         XPD_TIM_ChannelDisableIT(HANDLE, CH)\
    CLEAR_BIT((HANDLE)->Inst->DIER.w,(TIM_DIER_CC1IE << (CH)))

#define         XPD_TIM_ChannelEnableDMA(HANDLE, CH)\
    SET_BIT((HANDLE)->Inst->DIER.w,(TIM_DIER_CC1DE << (CH)))

#define         XPD_TIM_ChannelDisableDMA(HANDLE, CH)\
    CLEAR_BIT((HANDLE)->Inst->DIER.w,(TIM_DIER_CC1DE << (CH)))

#ifdef TIM_BB
#define TIM_REG_BIT(HANDLE, REG_NAME, BIT_NAME) (HANDLE->Inst_BB->REG_NAME.BIT_NAME)
#else
#define TIM_REG_BIT(HANDLE, REG_NAME, BIT_NAME) (HANDLE->Inst->REG_NAME.b.BIT_NAME)
#endif

/** @} */

/** @} */

/** @defgroup TIM_Output TIM Output
 * @{ */

/** @defgroup TIM_Output_Exported_Types TIM Output Exported Types
 * @{ */

/** @brief TIM output modes */
typedef enum
{
    TIM_OUTPUT_TIMING         = 0, /*!< Output channel is frozen, use for timing base generation */
    TIM_OUTPUT_ACTIVE         = 1, /*!< Output channel is active on match */
    TIM_OUTPUT_INACTIVE       = 2, /*!< Output channel is inactive on match */
    TIM_OUTPUT_TOGGLE         = 3, /*!< Output channel is toggled on match */
    TIM_OUTPUT_FORCEDINACTIVE = 4, /*!< Output channel is forced low */
    TIM_OUTPUT_FORCEDACTIVE   = 5, /*!< Output channel is forced high */
    TIM_OUTPUT_PWM1           = 6, /*!< Output channel is active when CNT < CCR */
    TIM_OUTPUT_PWM2           = 7  /*!< Output channel is active when CNT > CCR */
}TIM_OutputType;

/** @brief TIM output channel setup structure */
typedef struct
{
    TIM_OutputType Output;           /*!< Output channel mode
                                          @note Cannot be modified at LockLevel {3} */
    struct {
        ActiveLevelType ActiveLevel; /*!< Output channel active level
                                          @note Cannot be modified at LockLevel {2, 3} */
        FlagStatus      IdleState;   /*!< Output channel idle state
                                          @note Cannot be modified at LockLevel {1, 2, 3} */
    } Channel;
    struct {
        ActiveLevelType ActiveLevel; /*!< Complementary output channel active level
                                          @note Cannot be modified at LockLevel {2, 3} */
        FlagStatus      IdleState;   /*!< Complementary output channel idle state
                                          @note Cannot be modified at LockLevel {1, 2, 3} */
    } CompChannel;
} TIM_OutputChannelInitType;

/** @brief TIM Main Output break setup structure */
typedef union
{
    struct {
        uint8_t         LockLevel       : 2; /*!< [0 .. 3] Depending on value, locks selected configuration bits
                                                  @note Can only be set once after each reset */
        FunctionalState OffStateIdle    : 1; /*!< Off-state selection for Idle mode
                                                  @note Cannot be modified at LockLevel {1, 2, 3} */
        FunctionalState OffStateRun     : 1; /*!< Off-state selection for Run mode
                                                  @note Cannot be modified at LockLevel {2} */
        FunctionalState BreakState      : 1; /*!< Break function state
                                                  @note Cannot be modified at LockLevel {2} */
        ActiveLevelType BreakPolarity   : 1; /*!< Break input polarity
                                                  @note Cannot be modified at LockLevel {1} */
        FunctionalState AutomaticOutput : 2; /*!< Automatic output (MOE is set by software or update event)
                                                  @note Cannot be modified at LockLevel {1} */
    };
    uint8_t __BDTR1; /*!< [Internal] The byte value to place to BDTR register */
}TIM_OutputBreakType;

/** @} */

/** @addtogroup TIM_Output_Exported_Functions
 * @{ */
void            XPD_TIM_OutputEnable        (TIM_HandleType * htim);
void            XPD_TIM_OutputDisable       (TIM_HandleType * htim);

void            XPD_TIM_OutputInit          (TIM_HandleType * htim, TIM_ChannelType Channel,
                                             TIM_OutputChannelInitType* Config);
void            XPD_TIM_OutputStart         (TIM_HandleType * htim, TIM_ChannelType Channel);
void            XPD_TIM_OutputStop          (TIM_HandleType * htim, TIM_ChannelType Channel);
void            XPD_TIM_OutputStart_IT      (TIM_HandleType * htim, TIM_ChannelType Channel);
void            XPD_TIM_OutputStop_IT       (TIM_HandleType * htim, TIM_ChannelType Channel);
void            XPD_TIM_OutputStart_DMA     (TIM_HandleType * htim, TIM_ChannelType Channel,
                                             void * Address, uint16_t Length);
void            XPD_TIM_OutputStop_DMA      (TIM_HandleType * htim, TIM_ChannelType Channel);

void            XPD_TIM_OutputSetDeadtime   (TIM_HandleType * htim, uint32_t DeadCounts);
void            XPD_TIM_OutputBreakConfig   (TIM_HandleType * htim, TIM_OutputBreakType * Config);
/** @} */

/** @} */

/** @defgroup TIM_MasterSlave TIM Master-Slave
 * @{ */

/** @defgroup TIM_MasterSlave_Exported_Types TIM Master-Slave Exported Types
 * @{ */

/** @brief TIM trigger out types */
typedef enum
{
    TIM_TRGO_RESET  = 0, /*!< TRGO is connected to EGR.UG */
    TIM_TRGO_ENABLE = 1, /*!< TRGO is connected to the timer enable bit CR1.EN */
    TIM_TRGO_UPDATE = 2, /*!< TRGO is connected to the timer update */
    TIM_TRGO_OC1    = 3, /*!< TRGO is connected to the Channel 1 comparator on match */
    TIM_TRGO_OC1REF = 4, /*!< OC1REF is used for TRGO */
    TIM_TRGO_OC2REF = 5, /*!< OC2REF is used for TRGO */
    TIM_TRGO_OC3REF = 6, /*!< OC3REF is used for TRGO */
    TIM_TRGO_OC4REF = 7  /*!< OC4REF is used for TRGO */
}TIM_OutputTriggerType;

/** @brief TIM master setup structure */
typedef struct
{
    FunctionalState       MasterMode;    /*!< Master mode configuration */
    TIM_OutputTriggerType MasterTrigger; /*!< Trigger output (TRGO) selection */
}TIM_MasterConfigType;

/** @} */

/** @addtogroup TIM_MasterSlave_Exported_Functions
 * @{ */
void            XPD_TIM_MasterConfig        (TIM_HandleType * htim, TIM_MasterConfigType * Config);
/** @} */

/** @} */

/** @} */
#endif /* XPD_TIM_H_ */