#include "hw.h"

bool hwInit(void)
{
    bool ret = true;

    // ret &= cliInit();
    // ret &= logInit();
    // 
    // ret &= resetInit();
    ret &= bspInit();
    ret &= ledInit();
    //ret &= rtcInit();
    ret &= usbInit();
    ret &= usbBegin(USB_CDC_MODE);
    
    return ret;
}