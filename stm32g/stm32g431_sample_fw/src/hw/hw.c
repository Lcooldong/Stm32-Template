#include "hw.h"

bool hwInit(void)
{
    bool ret = true;

    // ret &= cliInit();
    // ret &= logInit();
    // ret &= rtcInit();
    // ret &= resetInit();
    ret &= bspInit();
    ret &= ledInit();

    return ret;
}