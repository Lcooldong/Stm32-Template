#include "ap.h"

uint32_t lastMillis = 0;
uint32_t currentMillis = 0;
int counter = 0;
uint8_t spi_txbuffer[4] = {0,};
uint8_t spi_rxbuffer[4] = {0,};

void apInit(void)
{
  delay(3000);
  spiFlashInit();
}

void apMain(void)
{
  while(1)
  {
    currentMillis = millis();
    

    if(uartAvailable(HW_UART_CH_DEBUG) > 0)
    {
      uint8_t text = uartRead(HW_UART_CH_DEBUG);
      
      switch (text)
      {
      case '1':
        uartPrintf(HW_UART_CH_DEBUG, "Button 1 pressed\r\n");
        break;
      case '2':
        uartPrintf(HW_UART_CH_DEBUG, "Button 2 pressed\r\n");
        break;
      case '3':
        logPrintf("Button 3 pressed\r\n");
        spi_txbuffer[0] = 0x9F;
        spiTransfer(_DEF_SPI1, spi_txbuffer, spi_rxbuffer, 4, 10);
        logPrintf("%X %X %X %X\r\n", spi_rxbuffer[0], spi_rxbuffer[1], spi_rxbuffer[2], spi_rxbuffer[3]);
      default:
        break;
      }
    }


    if(currentMillis - lastMillis >= 1000)
    {
      lastMillis = currentMillis;
      if(usbIsOpen() == true)
      {
        //  ledOn(_DEF_LED1);
        ledToggle(_DEF_LED1);
        counter++;
        uartPrintf(HW_UART_CH_DEBUG, "USB %d\r\n", counter);
      }
      else
      {
          ledOff(_DEF_LED1);
      }
    }

  }
}