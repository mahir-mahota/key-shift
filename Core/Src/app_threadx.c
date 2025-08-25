/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
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
#include "app_threadx.h"
#include "main.h"
/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define TX_THREAD_STACK_SIZE 1024

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t kp_matrix_thread_stack[TX_THREAD_STACK_SIZE];
TX_THREAD kp_matrix_thread;

/* Private function prototypes -----------------------------------------------*/
VOID kp_matrix_thread_entry(ULONG thread_input);

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  (void)memory_ptr;

  tx_thread_create(&kp_matrix_thread, "kp_matrix_thread", kp_matrix_thread_entry, 0, 
                   kp_matrix_thread_stack, TX_THREAD_STACK_SIZE, 15, 15, TX_NO_TIME_SLICE, TX_AUTO_START);

  return ret;
}

/**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  tx_kernel_enter();
}

VOID kp_matrix_thread_entry(ULONG thread_input) {
  int rows[NUM_ROWS] = {ROW1_Pin, ROW2_Pin, ROW3_Pin, ROW4_Pin};
  int columns[NUM_COLUMNS] = {COLUMN1_Pin, COLUMN2_Pin, COLUMN3_Pin, COLUMN4_Pin};

  while (1) {
    for (int r = 0; r < NUM_ROWS; r++) {
      HAL_GPIO_WritePin(GPIOA, rows[r], GPIO_PIN_RESET);
      tx_thread_sleep(5);

      for (int c = 0; c < NUM_COLUMNS; c++) {
        if (HAL_GPIO_ReadPin(GPIOA, columns[c]) == GPIO_PIN_RESET) {
          process_key(r, c);
        }
      }
      
      HAL_GPIO_WritePin(GPIOA, rows[r], GPIO_PIN_SET);
    }
    tx_thread_sleep(10);
  }
}