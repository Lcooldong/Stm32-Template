#include "ap.h"

void apInit(void)
{
}

void apMain(void)
{
    uint32_t pre_time[2];
    uint32_t length = sizeof(pre_time)/sizeof(pre_time[0]);

    for (uint32_t i = 0; i < length; i++)
    {
      pre_time[i] = millis();
    }
    
    while (1)   
    {
      uint32_t cur_time = millis();
      if(cur_time - pre_time[0] >= 500)
      {
        pre_time[0] = cur_time;
        ledToggle(_DEF_LED1);
      }

      if(cur_time - pre_time[1] >= 100)
      {
        pre_time[1] = cur_time;
        ledToggle(_DEF_LED2);
      }
    }
}