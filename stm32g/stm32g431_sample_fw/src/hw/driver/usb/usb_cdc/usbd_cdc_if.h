/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_cdc_if.h
  * @version        : v3.0_Cube
  * @brief          : Header for usbd_cdc_if.c file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CDC_IF_H__
#define __USBD_CDC_IF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc.h"


/* Define size for the receive and transmit buffer over CDC */
#define APP_RX_DATA_SIZE  1024
#define APP_TX_DATA_SIZE  1024

/** CDC Interface callback. */
extern USBD_CDC_ItfTypeDef USBD_CDC_fops;




/** @defgroup USBD_CDC_IF_Exported_FunctionsPrototype USBD_CDC_IF_Exported_FunctionsPrototype
  * @brief Public functions declaration.
  * @{
  */

uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);

/* USER CODE BEGIN EXPORTED_FUNCTIONS */
bool     cdcIfInit(void);
uint32_t cdcIfAvailable(void);
uint8_t  cdcIfRead(void);
uint32_t cdcIfGetBaud(void);
uint32_t cdcIfWrite(uint8_t *p_data, uint32_t length);
bool     cdcIfIsConnected(void);
uint8_t  cdcIfGetType(void);
/* USER CODE END EXPORTED_FUNCTIONS */



#ifdef __cplusplus
}
#endif

#endif /* __USBD_CDC_IF_H__ */

