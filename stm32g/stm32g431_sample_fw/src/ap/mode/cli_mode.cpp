#include "cli_mode.h"

bool cliModeInit(void)
{
  return true;
}

void cliModeMain(mode_args_t *args)
{
  logPrintf("cliMode in\r\n");
  uint32_t cli_pre_time = millis();
  while(args->keepLoop())
  {
    if(millis() - cli_pre_time >= 2)
    {
      cli_pre_time = millis();
      cliMain();
    }
    else
    {

    }
    // cliMain();
    // delay(2);
    
  }

  logPrintf("cliMode out\r\n");

}