#ifndef DYNAMIXEL_H_
#define DYNAMIXEL_H_

#include "hw_def.h"

#ifdef __cplusplus
extern "C" {
#endif



// Protocol version
#define PROTOCOL_VERSION                2.0                 // See which protocol version is used in the Dynamixel

// Default setting
#define DXL_ID                          1                   // Dynamixel ID: 1
#define BAUDRATE                        57600
#define DEVICENAME                      "/dev/ttyUSB0"      // Check which port is being used on your controller
                                                            // ex) Windows: "COM1"   Linux: "/dev/ttyUSB0" Mac: "/dev/tty.usbserial-*"

void dynamixel_init(uint8_t ch);


#ifdef __cplusplus
}
#endif

#endif