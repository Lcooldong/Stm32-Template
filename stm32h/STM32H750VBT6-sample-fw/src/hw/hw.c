#include "hw.h"

bool hwInit(void)
{
    bool ret = true;

   
    
    // ret &= bspInit();
    // ret &= cliInit();
    

    // ret &= rtcInit();
    // ret &= resetInit();
    ret &= ledInit();
     
    // ret &= logOpen(_DEF_UART1, 115200);  // uartOpen 포함

    // if(resetGetCount() == 2)
    // {
    //     resetToSysBoot();
    // }

    // logPrintf("[ Firmware Begin... ]\r\n");
    // logPrintf("Booting..Name \t\t: %s\r\n", _DEF_BOARD_NAME);
    // logPrintf("Booting..Ver  \t\t: %s\r\n", _DEF_FIRMWATRE_VERSION);

    logInit();  // cli 밑에
    uartInit();

    logOpen(_DEF_UART4, 115200);
    
    // ret &= uartOpen(_DEF_UART4, 115200);
    
    usbInit();
    usbBegin(USB_CDC_MODE);
    // ret &= canInit();
    // ret &= i2cInit();
    
    // logBoot(false);

    return ret;
}