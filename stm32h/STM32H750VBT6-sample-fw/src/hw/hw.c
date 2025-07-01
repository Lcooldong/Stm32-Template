#include "hw.h"

bool hwInit(void)
{
    bool ret = true;

    ret &= ledInit();
    
    logInit(); 
    logOpen(HW_UART_CH_USB, 115200); 
    usbInit();
    usbBegin(USB_CDC_MODE);
    uartInit();
    for (int i = 0; i < HW_UART_MAX_CH; i++)
    {
        uartOpen(i, 115200);
    }
    
    logBoot(false);

    return ret;
}