/*
 * usb.h
 *
 *  Created on: 2021. 6. 18.
 *      Author: baram
 */

 #ifndef SRC_COMMON_HW_INCLUDE_USB_H_
 #define SRC_COMMON_HW_INCLUDE_USB_H_
 
 #include "hw_def.h"
 
 
 #ifdef _USE_HW_USB
 
 
 typedef enum
 {
   USB_NON_MODE,
   USB_CDC_MODE,
   USB_MSC_MODE
 } UsbMode_t;
 
 typedef enum UsbType
 {
   USB_CON_CDC = 0,
   USB_CON_CLI = 1,
   USB_CON_CAN = 2,
   USB_CON_ESP = 3,
 } UsbType_t;
 
 bool usbInit(void);
 bool usbBegin(UsbMode_t usb_mode);
 void usbDeInit(void);
 bool usbIsOpen(void);
 bool usbIsConnect(void);
 
 UsbMode_t usbGetMode(void);
 UsbType_t usbGetType(void);
 
 
 #endif
 
 #endif /* SRC_COMMON_HW_INCLUDE_USB_H_ */