#include "hw.h"

bool hwInit(void)
{
    bool ret = true;

   
    
    ret &= bspInit();
    ret &= cliInit();
    ret &= ledInit();
    ret &= rtcInit();
    ret &= resetInit();

    if(resetGetCount() == 2)
    {
        resetToSysBoot();
    }


    ret &= usbInit();
    ret &= usbBegin(USB_CDC_MODE);
    ret &= uartInit();
    //ret &= uartOpen(_DEF_UART1, 57600);
    ret &= logInit();   
    ret &= logOpen(_DEF_UART1, 57600);  // uartOpen 포함

    

    return ret;
}