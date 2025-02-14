#include "ap.h"

typedef enum
{
  MODE_IDLE,
  MODE_CLI,
  MODE_CAN,
}ap_mode_t;

static ap_mode_t mode = MODE_IDLE;
static ap_mode_t mode_next = MODE_IDLE;
// static mode_args_t mode_args;

void apInit(void)
{
  cliOpen(_DEF_UART1, 57600);
}

void apMain(void)
{
    uint32_t pre_time[2];
    uint32_t length = sizeof(pre_time)/sizeof(pre_time[0]);
    int count = 0;

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
        count++;
        // logPrintf("[%d]:\r\n", count);
      }

      
      if (cur_time - pre_time[1] >= 500)
      {
        pre_time[1] = cur_time;
        //if (cdcIsConnect() == true)
        if (usbIsOpen() == true)
        {
          ledToggle(_DEF_LED2);
        }
        else
        {
          ledOff(_DEF_LED2);
        }
      }


      // if (cdcAvailable() > 0)
      // {
      //   uint8_t rx_data;
      //   const char *startText = "RX : ";
      //   const char *endText = "\r\n";

      //   rx_data = cdcRead();

      //   cdcWrite((uint8_t *)startText, sizeof(startText));
      //   cdcWrite(&rx_data, 1);
      //   cdcWrite((uint8_t *)endText, sizeof(endText));
      // }

      // if(uartAvailable(_DEF_UART1) > 0)
      // {
      //   uint8_t rx_data;

      //   rx_data = uartRead(_DEF_UART1);
      //   uartPrintf(_DEF_UART1, "RX : 0x%X\r\n", rx_data);
      // }

      cliMain();

    }
}