#ifndef __AP_H__
#define __AP_H__

#include "ap_def.h"

// #include "mode/cli_mode.h"
// #include "mode/can_mode.h"

typedef enum
{
  MODE_IDLE,
  MODE_CLI,
  MODE_CAN,
} ap_mode_t;

void apInit(void);
void apMain(void);


#endif


