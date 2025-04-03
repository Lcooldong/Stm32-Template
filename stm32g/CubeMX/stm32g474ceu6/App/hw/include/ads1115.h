#ifndef ADS1115_H_
#define ADS1115_H_

#include "hw_def.h"


#ifdef __cplusplus
extern "C" {
#endif




#define ADS1X15_LIB_VERSION               (F("0.5.2"))

//  allow compile time default address
//  address in { 0x48, 0x49, 0x4A, 0x4B }, no test...
#ifndef ADS1015_ADDRESS
#define ADS1015_ADDRESS                   0x48
#endif

#ifndef ADS1115_ADDRESS
#define ADS1115_ADDRESS                   0x48
#endif


#define ADS1X15_OK                        0
#define ADS1X15_INVALID_VOLTAGE           -100
#define ADS1X15_ERROR_TIMEOUT             -101
#define ADS1X15_ERROR_I2C                 -102
#define ADS1X15_INVALID_GAIN              0xFF
#define ADS1X15_INVALID_MODE              0xFE


//  PARAMETER CONSTANTS NOT USED IN CODE YET
//  enum ?
#define ADS1X15_GAIN_6144MV               0x00
#define ADS1X15_GAIN_4096MV               0x01
#define ADS1X15_GAIN_2048MV               0x02
#define ADS1X15_GAIN_1024MV               0x04
#define ADS1X15_GAIN_0512MV               0x08
#define ADS1X15_GAIN_0256MV               0x10

#define ADS1x15_COMP_MODE_TRADITIONAL     0x00
#define ADS1x15_COMP_MODE_WINDOW          0x01

#define ADS1x15_COMP_POL_FALLING_EDGE     0x00
#define ADS1x15_COMP_POL_RISING_EDGE      0x01

#define ADS1x15_COMP_POL_LATCH            0x00
#define ADS1x15_COMP_POL_NOLATCH          0x01



#define ADS1115_CONVERSION_DELAY    8
//  REGISTERS
#define ADS1X15_REG_CONVERT         0x00
#define ADS1X15_REG_CONFIG          0x01
#define ADS1X15_REG_LOW_THRESHOLD   0x02
#define ADS1X15_REG_HIGH_THRESHOLD  0x03

//  CONFIG REGISTER

//  BIT 15      Operational Status           //  1 << 15
#define ADS1X15_OS_BUSY             0x0000
#define ADS1X15_OS_NOT_BUSY         0x8000
#define ADS1X15_OS_START_SINGLE     0x8000

//  BIT 12-14   read differential
#define ADS1X15_MUX_DIFF_0_1        0x0000
#define ADS1X15_MUX_DIFF_0_3        0x1000
#define ADS1X15_MUX_DIFF_1_3        0x2000
#define ADS1X15_MUX_DIFF_2_3        0x3000
//              read single
#define ADS1X15_READ_0              0x4000   //  pin << 12
#define ADS1X15_READ_1              0x5000   //  pin = 0..3
#define ADS1X15_READ_2              0x6000
#define ADS1X15_READ_3              0x7000


//  BIT 9-11    gain                         //  (0..5) << 9
#define ADS1X15_PGA_6_144V          0x0000   //  voltage
#define ADS1X15_PGA_4_096V          0x0200   //
#define ADS1X15_PGA_2_048V          0x0400   //  default
#define ADS1X15_PGA_1_024V          0x0600
#define ADS1X15_PGA_0_512V          0x0800
#define ADS1X15_PGA_0_256V          0x0A00

//  BIT 8       mode                         //  1 << 8
#define ADS1X15_MODE_CONTINUE       0x0000
#define ADS1X15_MODE_SINGLE         0x0100

//  BIT 5-7     data rate sample per second  //  (0..7) << 5
/*
differs for different devices, check datasheet or readme.md

|  data rate  |  ADS101x  |  ADS111x  |   Notes   |
|:-----------:|----------:|----------:|:---------:|
|     0       |   128     |    8      |  slowest  |
|     1       |   250     |    16     |           |
|     2       |   490     |    32     |           |
|     3       |   920     |    64     |           |
|     4       |   1600    |    128    |  default  |
|     5       |   2400    |    250    |           |
|     6       |   3300    |    475    |           |
|     7       |   3300    |    860    |  fastest  |
*/

//  BIT 4 comparator modi                    //  1 << 4
#define ADS1X15_COMP_MODE_TRADITIONAL   0x0000
#define ADS1X15_COMP_MODE_WINDOW        0x0010

//  BIT 3 ALERT active value                 //  1 << 3
#define ADS1X15_COMP_POL_ACTIV_LOW      0x0000
#define ADS1X15_COMP_POL_ACTIV_HIGH     0x0008

//  BIT 2 ALERT latching                     //  1 << 2
#define ADS1X15_COMP_NON_LATCH          0x0000
#define ADS1X15_COMP_LATCH              0x0004

//  BIT 0-1 ALERT mode                       //  (0..3)
#define ADS1X15_COMP_QUE_1_CONV         0x0000  //  trigger alert after 1 convert
#define ADS1X15_COMP_QUE_2_CONV         0x0001  //  trigger alert after 2 converts
#define ADS1X15_COMP_QUE_4_CONV         0x0002  //  trigger alert after 4 converts
#define ADS1X15_COMP_QUE_NONE           0x0003  //  disable comparator


//  _CONFIG masks
//
//  |  bit  |  description           |
//  |:-----:|:-----------------------|
//  |   0   |  # channels            |
//  |   1   |  -                     |
//  |   2   |  resolution            |
//  |   3   |  -                     |
//  |   4   |  GAIN supported        |
//  |   5   |  COMPARATOR supported  |
//  |   6   |  -                     |
//  |   7   |  -                     |
//
#define ADS_CONF_CHAN_1  0x00
#define ADS_CONF_CHAN_4  0x01
#define ADS_CONF_RES_12  0x00
#define ADS_CONF_RES_16  0x04
#define ADS_CONF_NOGAIN  0x00
#define ADS_CONF_GAIN    0x10
#define ADS_CONF_NOCOMP  0x00
#define ADS_CONF_COMP    0x20




typedef enum
{
  ADS1115_ADDRESS_1 = 0x48,
  ADS1115_ADDRESS_2 = 0x49,
  ADS1115_ADDRESS_3 = 0x4A,
  ADS1115_ADDRESS_4 = 0x4B
}address_t;

typedef struct
{
  uint8_t  _config;
  uint8_t  _maxPorts;
  uint8_t  _address;
  uint8_t  _conversionDelay;
  uint8_t  _bitShift;
  uint16_t _gain;
  uint16_t _mode;
  uint16_t _datarate;

  uint32_t  _clockSpeed;
  uint16_t _lastRequest;

}ads_config_t;


typedef struct
{
  uint8_t  _compMode;
  uint8_t  _compPol;
  uint8_t  _compLatch;
  uint8_t  _compQueConvert;

}ads_comp_t;

typedef struct
{
  uint16_t ain0;
  uint16_t ain1;
  uint16_t ain2;
  uint16_t ain3;

}ads1115_data_t;

typedef union 
{
  ads1115_data_t ain_data;
  uint16_t ains[4];
  
}ads1115_packet_t;



typedef struct
{
  uint8_t        _ch;
  ads_config_t   *ads_config;
  ads_comp_t     *ads_comp;
  ads1115_packet_t ads_packet;
  int8_t         _error;

}ads_tbl_t;



void ADS1115_init(uint8_t address);
void ADS1115_reset();
bool ADS1115_begin(uint8_t ch);
bool ADS1115_isConnected();

void setGain(uint8_t gain);          //  6.144 volt
void setDataRate(uint8_t dataRate);  //  0 = slow   4 = medium   7 = fast
void setMode(uint8_t mode);          //  continuous mode
uint16_t readADC(uint8_t pin);       //  first read to trigger
uint16_t getValue();

#ifdef __cplusplus
}
#endif
#endif