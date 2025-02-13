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
      if(cur_time - pre_time[0] >= 1000)
      {
        pre_time[0] = cur_time;
        ledToggle(_DEF_LED1);
      }

      
      if (cur_time - pre_time[1] >= 100)
      {
        pre_time[1] = cur_time;
        if (cdcIsConnect() == true)
        {
          ledToggle(_DEF_LED2);
        }
        else
        {
          ledOff(_DEF_LED2);
        }
      }


      if (cdcAvailable() > 0)
      {
        uint8_t rx_data;
        const char *startText = "RX : ";
        const char *endText = "\r\n";

        rx_data = cdcRead();

        cdcWrite((uint8_t *)startText, sizeof(startText));
        cdcWrite(&rx_data, 1);
        cdcWrite((uint8_t *)endText, sizeof(endText));
      }

    }
}