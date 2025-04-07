#include "ap.h"

void apInit(void)
{

}


void apMain(void)
{
  uint32_t pre_time;
  uint32_t count = 0;

  pre_time = millis();
  

  while(1)
  {  
    if (millis() - pre_time >= 1000)
    {
      pre_time = millis();
      count++;
      HAL_GPIO_TogglePin(LED_BUILTIN_GPIO_Port, LED_BUILTIN_Pin);
    }
  }
}