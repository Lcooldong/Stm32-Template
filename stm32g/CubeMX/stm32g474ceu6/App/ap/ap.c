#include "ap.h"

extern ads_tbl_t ads_tbl;
extern ads1115_packet_t ads1115_packet;

void apInit(void)
{
  ADS1115_begin(_DEF_I2C1);
}


void apMain(void)
{
  uint32_t pre_time;
  int targetUART = _DEF_UART3;
  int count = 0;
  uint8_t i2c_data[2] = {0,};
  uint8_t i2c_request[2] = {0xC0, 0xE8};


  uint16_t a1 = 0x8000;
  a1 |= 0x4000;

  pre_time = millis();


  while(1)
  {  
    if (millis() - pre_time >= 100)
    {
      pre_time = millis();
      ledToggle(_DEF_LED1);
      count++;

      // i2cWriteBytes(_DEF_I2C1, 0x48, 0x01, i2c_request, sizeof(i2c_request), 100);
      // i2cReadBytes(_DEF_I2C1, 0x48, 0x00, i2c_data, 2, 100);
      uartPrintf(targetUART, "[%d]  %d %d\r\n", count , getValue(), ads1115_packet.ains[0]);

 
    }

    
    if (uartAvailable(targetUART))
    {
      uartPrintf(targetUART, "rx : 0x%X\r\n", uartRead(targetUART));
    }
  }
} 