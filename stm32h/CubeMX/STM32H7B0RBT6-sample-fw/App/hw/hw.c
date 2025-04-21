#include "hw.h"


// extern FDCAN_HandleTypeDef hfdcan1;

bool hwInit(void)
{
  
  ledInit();
  // canMXInit(hfdcan1, _DEF_CAN1);

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