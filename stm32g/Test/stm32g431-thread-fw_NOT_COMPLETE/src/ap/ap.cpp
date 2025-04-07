#include "ap.h"

void cliUpdate(void);

void apInit(void)
{
  // threadInit();
  
  cliOpen(HW_LOG_CH, 115200);
  // canOpen(_DEF_CAN1, CAN_NORMAL, CAN_FD_NO_BRS, CAN_500K, CAN_2M);
  canOpen(_DEF_CAN1, CAN_NORMAL, CAN_CLASSIC, CAN_125K, CAN_125K);  // Sync to uart 115200
  i2cOpen(_DEF_I2C2, I2C_FREQ_100KHz);
  logBoot(false);
}

void apMain(void)
{
  uint32_t pre_time ;
  pre_time = millis();

  while(1)
  {
    if(millis() - pre_time >= 500)
    {
      pre_time = millis();

      if(usbIsOpen() == true)
      {
        ledToggle(_DEF_LED1);
        ledToggle(_DEF_LED2);
      }
      else 
      {
        ledOff(_DEF_LED1);
        ledOff(_DEF_LED2);
      }
      // ledToggle(_DEF_LED2);
    }
    
    cliUpdate();

    // threadUpdate();
  }
}

void cliUpdate(void)
{
  uint8_t cli_ch;

  if(usbIsOpen() && usbGetType() == USB_CON_CLI)
  {
    cli_ch = HW_UART_CH_USB;
  }
  else
  {
    cli_ch = HW_UART_CH_DEBUG;
  }
  if(cli_ch != cliGetPort())
  {
    cliOpen(cli_ch, 0);
  }
  cliMain();
}