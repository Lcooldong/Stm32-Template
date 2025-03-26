#include "ap.h"

#include "usbd_conf.h"

extern PCD_HandleTypeDef hpcd_USB_OTG_HS;

void apInit(void)
{

}


void apMain(void)
{
  uint32_t pre_time;

  pre_time = millis();
  while(1)
  {  
    if (millis() - pre_time >= 500)
    {
      pre_time = millis();
      
    }

    

    if(buttonGetPressed(_DEF_BUTTON1) == GPIO_PIN_SET)
    // if(HAL_PCD_DevConnect(&hpcd_USB_OTG_HS) == HAL_OK)
    {
      ledOn(_DEF_LED1);
    }
    else
    {
      ledOff(_DEF_LED1);
    }

    


  }
} 