/**
  ******************************************************************************
  * @file    app_usbx_device.h
  * @author  MCD Application Team
  * @brief   USBX Device applicative header file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_USBX_DEVICE_H__
#define __APP_USBX_DEVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"
#include "ux_device_descriptors.h"
#include "ux_device_keyboard.h"
#include "ux_device_cdc_acm.h"

#include "usb_otg.h"
#include "ux_dcd_stm32.h"
/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define USBX_DEVICE_MEMORY_STACK_SIZE     14*1024

#define UX_DEVICE_APP_THREAD_STACK_SIZE   1024
#define UX_DEVICE_APP_THREAD_PRIO         10
/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
UINT MX_USBX_Device_Init(VOID *memory_ptr);
VOID USBX_APP_Device_Init(VOID);

/* Private defines -----------------------------------------------------------*/
#ifndef UX_DEVICE_APP_THREAD_NAME
#define UX_DEVICE_APP_THREAD_NAME  "USBX Device App Main Thread"
#endif

#ifndef UX_DEVICE_APP_THREAD_PREEMPTION_THRESHOLD
#define UX_DEVICE_APP_THREAD_PREEMPTION_THRESHOLD  UX_DEVICE_APP_THREAD_PRIO
#endif

#ifndef UX_DEVICE_APP_THREAD_TIME_SLICE
#define UX_DEVICE_APP_THREAD_TIME_SLICE  TX_NO_TIME_SLICE
#endif

#ifndef UX_DEVICE_APP_THREAD_START_OPTION
#define UX_DEVICE_APP_THREAD_START_OPTION  TX_AUTO_START
#endif

#ifdef __cplusplus
}
#endif
#endif /* __APP_USBX_DEVICE_H__ */
