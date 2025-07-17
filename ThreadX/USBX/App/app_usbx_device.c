/**
  ******************************************************************************
  * @file    app_usbx_device.c
  * @author  MCD Application Team
  * @brief   USBX Device applicative file
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

/* Includes ------------------------------------------------------------------*/
#include "app_usbx_device.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static ULONG hid_keyboard_interface_number;
static ULONG hid_keyboard_configuration_number;
static UX_SLAVE_CLASS_HID_PARAMETER hid_keyboard_parameter;
static TX_THREAD ux_device_app_thread;

TX_THREAD ux_hid_keyboard_thread;

/* Private function prototypes -----------------------------------------------*/
static VOID app_ux_device_thread_entry(ULONG thread_input);


/**
  * @brief Application USBX Device Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT MX_USBX_Device_Init(VOID *memory_ptr)
{
  UINT ret = UX_SUCCESS;
  UCHAR *device_framework_high_speed;
  UCHAR *device_framework_full_speed;
  ULONG device_framework_hs_length;
  ULONG device_framework_fs_length;
  ULONG string_framework_length;
  ULONG language_id_framework_length;
  UCHAR *string_framework;
  UCHAR *language_id_framework;
  UCHAR *pointer;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       USBX_DEVICE_MEMORY_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Initialize USBX Memory */
  if (ux_system_initialize(pointer, USBX_DEVICE_MEMORY_STACK_SIZE, UX_NULL, 0) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Get Device Framework High Speed and get the length */
  device_framework_high_speed = USBD_Get_Device_Framework_Speed(USBD_HIGH_SPEED,
                                                                &device_framework_hs_length);

  /* Get Device Framework Full Speed and get the length */
  device_framework_full_speed = USBD_Get_Device_Framework_Speed(USBD_FULL_SPEED,
                                                                &device_framework_fs_length);

  /* Get String Framework and get the length */
  string_framework = USBD_Get_String_Framework(&string_framework_length);

  /* Get Language Id Framework and get the length */
  language_id_framework = USBD_Get_Language_Id_Framework(&language_id_framework_length);

  /* Install the device portion of USBX */
  if (ux_device_stack_initialize(device_framework_high_speed,
                                 device_framework_hs_length,
                                 device_framework_full_speed,
                                 device_framework_fs_length,
                                 string_framework,
                                 string_framework_length,
                                 language_id_framework,
                                 language_id_framework_length,
                                 UX_NULL) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Initialize the hid keyboard class parameters for the device */
  hid_keyboard_parameter.ux_slave_class_hid_instance_activate         = USBD_HID_Keyboard_Activate;
  hid_keyboard_parameter.ux_slave_class_hid_instance_deactivate       = USBD_HID_Keyboard_Deactivate;
  hid_keyboard_parameter.ux_device_class_hid_parameter_report_address = USBD_Get_Device_HID_ReportDesc();
  hid_keyboard_parameter.ux_device_class_hid_parameter_report_length  = (ULONG)USBD_HID_ReportDesc_length();
  hid_keyboard_parameter.ux_device_class_hid_parameter_report_id      = UX_FALSE;
  hid_keyboard_parameter.ux_device_class_hid_parameter_callback       = USBD_HID_Keyboard_SetReport;
  hid_keyboard_parameter.ux_device_class_hid_parameter_get_callback   = USBD_HID_Keyboard_GetReport;

  /* Get hid keyboard configuration number */
  hid_keyboard_configuration_number = 1;

  /* Find hid keyboard interface number */
  hid_keyboard_interface_number = USBD_Get_Interface_Number(CLASS_TYPE_HID);

  /* Initialize the device hid keyboard class */
  if (ux_device_stack_class_register(_ux_system_slave_class_hid_name,
                                     ux_device_class_hid_entry,
                                     hid_keyboard_configuration_number,
                                     hid_keyboard_interface_number,
                                     &hid_keyboard_parameter) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Allocate the stack for device application main thread */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, UX_DEVICE_APP_THREAD_STACK_SIZE,
                       TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the device application main thread */
  if (tx_thread_create(&ux_device_app_thread, UX_DEVICE_APP_THREAD_NAME, app_ux_device_thread_entry,
                       0, pointer, UX_DEVICE_APP_THREAD_STACK_SIZE, UX_DEVICE_APP_THREAD_PRIO,
                       UX_DEVICE_APP_THREAD_PREEMPTION_THRESHOLD, UX_DEVICE_APP_THREAD_TIME_SLICE,
                       UX_DEVICE_APP_THREAD_START_OPTION) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

    /* Allocate the stack for usbx hid keyboard thread */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, 1024, TX_NO_WAIT) != UX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the  usbx hid keyboard thread */
  if (tx_thread_create(&ux_hid_keyboard_thread, "ux_hid_keyboard_thread_read_entry",
                       usbx_hid_keyboard_thread_entry, 0, pointer, 1024,
                       20, 20, TX_NO_TIME_SLICE, TX_AUTO_START) != UX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }
  return ret;
}

/**
  * @brief  Function implementing app_ux_device_thread_entry.
  * @param  thread_input: User thread input parameter.
  * @retval none
  */
static VOID app_ux_device_thread_entry(ULONG thread_input)
{
  UNUSED(thread_input);
  /* Initialization of USB device */
  USBX_APP_Device_Init();
}

/**
  * @brief  USBX_APP_Device_Init
  *         Initialization of USB device.
  * @param  none
  * @retval none
  */
VOID USBX_APP_Device_Init(VOID)
{
  /* USB_OTG_HS init function */
  MX_USB_OTG_FS_PCD_Init();

  /* Set Rx FIFO */
  HAL_PCDEx_SetRxFiFo(&hpcd_USB_OTG_FS, 0x200);
  /* Set Tx FIFO 0 */
  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 0, 0x80);
  /* Set Tx FIFO 1 */
  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 1, 0x40);
  /* Set Tx FIFO 2 */
  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 2, 0x40);

  /* Initialize the device controller driver */
  ux_dcd_stm32_initialize((ULONG)USB_OTG_FS, (ULONG)&hpcd_USB_OTG_FS);

  /* Start USB device */
  HAL_PCD_Start(&hpcd_USB_OTG_FS);
}