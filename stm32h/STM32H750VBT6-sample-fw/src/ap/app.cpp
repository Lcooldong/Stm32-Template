#include "ap.h"
#include "usbd_cdc_if.h"

int counter = 0;
uint32_t lastMilis = 0;
uint32_t currMilis = 0;

void apInit(void)
{
    // cdcInit();
    
    logBoot(false);
}

void apMain(void)
{
   
    while (1)
    {
        if(cdcAvailable() > 0)
        {
            // uint8_t rx_data = cdcRead();
            uint8_t rx_data = uartRead(HW_UART_CH_USB);
            logPrintf("%c", rx_data);
        }
    
        currMilis = millis();
        if(currMilis - lastMilis >= 1000)
        {
            lastMilis = currMilis;
            counter++;
            logPrintf("[%d] %d\r\n", counter, usbGetType());
        }

        if(usbIsOpen() == true && usbGetType() == USB_CON_CLI)  // USB_CON_CLI -> 115200 LineCoding
        {
            ledOn(_DEF_LED1);
        }
        else
        {
           ledOff(_DEF_LED1);
        }
       
    }
}