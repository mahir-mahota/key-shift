/**
  ******************************************************************************
  * @file    ux_device_keyboard.h
  * @author  MCD Application Team
  * @brief   USBX Device HID Keyboard applicative header file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UX_DEVICE_KEYBOARD_H__
#define __UX_DEVICE_KEYBOARD_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"
#include "ux_device_class_hid.h"

/* Private includes ----------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
VOID USBD_HID_Keyboard_Activate(VOID *hid_instance);
VOID USBD_HID_Keyboard_Deactivate(VOID *hid_instance);
UINT USBD_HID_Keyboard_SetReport(UX_SLAVE_CLASS_HID *hid_instance,
                                 UX_SLAVE_CLASS_HID_EVENT *hid_event);
UINT USBD_HID_Keyboard_GetReport(UX_SLAVE_CLASS_HID *hid_instance,
                                 UX_SLAVE_CLASS_HID_EVENT *hid_event);

VOID usbx_hid_keyboard_thread_entry(ULONG thread_input);

/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif  /* __UX_DEVICE_KEYBOARD_H__ */
