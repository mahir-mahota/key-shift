
/**
  ******************************************************************************
  * @file    app_eclipse_rtos_config.h
  * @author  MCD Application Team
  * @brief   eclipse_rtos config header file
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
#ifndef APP_ECLIPSE_RTOS_CONFIG_H
#define APP_ECLIPSE_RTOS_CONFIG_H
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* define the size of static threadX byte memory pools */
#define TX_APP_MEM_POOL_SIZE                     1024
#define UX_DEVICE_APP_MEM_POOL_SIZE              16*1024

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* APP_ECLIPSE_RTOS_CONFIG_H */
