#include "hw.h"




bool hwInit(void)
{
  
  ledInit();
  usbInit();
  usbBegin(USB_CDC_MODE);
  // gpioInit();
  // buttonInit();
  // swtimerInit();
  // buttonInit();
  // cdcInit();


  return true;
}

void delay(uint32_t ms)
{
  HAL_Delay(ms);
}

uint32_t millis(void)
{
  return HAL_GetTick();
}