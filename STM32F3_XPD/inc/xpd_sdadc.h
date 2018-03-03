/**
  ******************************************************************************
  * @file    xpd_sdadc.h
  * @author  Benedek Kupper
  * @version 0.1
  * @date    2018-01-28
  * @brief   STM32 eXtensible Peripheral Drivers CEC Module
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
#ifndef __XPD_SDADC_H_
#define __XPD_SDADC_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <xpd_common.h>

#define XPD_SDADC_API
#include <xpd_rcc_pc.h>
#undef XPD_SDADC_API

#ifdef __cplusplus
}
#endif

#endif /* __XPD_SDADC_H_ */
