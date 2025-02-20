#include "can_mode.h"

bool canModeInit(void)
{
  return true;
}

void canModeMain(mode_args_t *args)
{
  logPrintf("canMode in\r\n");
  uint32_t can_pre_time = millis();
  while (args->keepLoop())
  {

    if(millis() - can_pre_time >= 1)
    {
      can_pre_time = millis();
    }
    else if(uartAvailable(_DEF_UART1) > 0)
    {
      uartPrintf(_DEF_UART1, "RX : 0x%X\r\n", uartRead(_DEF_UART1));
    }

  }
  
  logPrintf("canMode out\r\n");
}