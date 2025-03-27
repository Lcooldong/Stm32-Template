#include "ap.h"

#include "main.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
extern USBD_HandleTypeDef hUsbDeviceHS;

// bool is_connected = false;

// void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd)
// {
//   is_connected = true;
// }

// void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd)
// {
//   is_connected = false;
// }

void apInit(void)
{
  
}


void apMain(void)
{
  uint32_t pre_time;
  pre_time = millis();
  char buffer[32];
  int count = 0;
  
  while(1)
  {  
    if (millis() - pre_time >= 500)
    {
      pre_time = millis();
      sprintf(buffer, "[%d]: %d \r\n", count++, strlen(buffer));
      CDC_Transmit_HS((uint8_t*)buffer, strlen(buffer));
    }

    if(buttonGetPressed(_DEF_BUTTON1) == GPIO_PIN_SET)
    {
      ledOn(_DEF_LED1);
    }
    else
    {
      ledOff(_DEF_LED1);
    }

  }
} 