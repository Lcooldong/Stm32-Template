#include "ap.h"
#include "usbd_cdc_if.h"

int counter = 0;

void apInit(void)
{
    
}

void apMain(void)
{
   
    while (1)
    {
        ledToggle(_DEF_LED1);
        logPrintf("Hello World!\r\n");
        
        delay(500);
    }
}