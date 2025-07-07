#include "hw.h"



bool hwInit(void)
{
  
  bspInit();
  logInit();
  uartInit();

  logOpen(HW_UART_CH_DEBUG, 115200);

  ledInit();

  
  // uartOpen(HW_UART_CH_DEBUG, 115200);
  usbInit();
  usbBegin(USB_CDC_MODE);

  spiInit();
  spiBegin(_DEF_SPI1);
  
  
  logBoot(false);

  return true;
}