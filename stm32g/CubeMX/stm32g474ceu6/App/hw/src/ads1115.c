#include "ads1115.h"
#include "qbuffer.h"
#include "i2c.h"
#include "uart.h"






// typedef struct
// {
//   uint8_t        _ch;
//   ads_config_t   *ads_config;
//   ads_comp_t     *ads_comp;
//   ads1115_packet ads_packet;
//   int8_t         _error;

// }ads_tbl_t;

ads1115_packet_t ads1115_packet = {0,};
ads_config_t ads_config;
ads_comp_t ads_comp;
ads_tbl_t ads_tbl;



void ADS1115_reset()
{
  setGain(0);
  setMode(1);
  setDataRate(4);

  ads_tbl.ads_comp->_compMode  = 0;
  ads_tbl.ads_comp->_compPol   = 1;
  ads_tbl.ads_comp->_compLatch = 0;
  ads_tbl.ads_comp->_compQueConvert = 3;
  

  ads_tbl._error = ADS1X15_OK;
}


void ADS1115_init(uint8_t address)
{
  ads_tbl._ch = address;
  ads_tbl.ads_config = &ads_config;
  ads_tbl.ads_comp = &ads_comp;

  ads_tbl.ads_config->_address = 0x48;
  ads_tbl.ads_config->_config = ADS_CONF_COMP | ADS_CONF_GAIN | ADS_CONF_RES_16 | ADS_CONF_CHAN_4;
  ads_tbl.ads_config->_conversionDelay = ADS1115_CONVERSION_DELAY;
  ads_tbl.ads_config->_bitShift = 0;
  ads_tbl.ads_config->_maxPorts = 4;

  
  // ads_tbl.ads_config->_clockSpeed = 100000; 일단은 고정을 해놓음
}

bool ADS1115_begin(uint8_t ch)
{
  ADS1115_init(ch);

  if(!i2cBegin(ch, I2C_FREQ_100KHz))       return false;
  if((ads_tbl.ads_config->_address < 0x48) || (ads_tbl.ads_config->_address > 0x4B)) return false;
  if(i2cIsDeviceReady(ch, ads_tbl.ads_config->_address))
  {
  }       
  else
  {
    return false;
  }
  // ads_tbl._ch = ch;

  setGain(0);      //  6.144 volt
  setDataRate(7);  //  0 = slow   4 = medium   7 = fast
  setMode(0);      //  continuous mode
  readADC(0);      //  first read to trigger
  
  return true;
}

// bool ADS1115_isConnected()
// {

//   return ;
// }

void setGain(uint8_t gain)
{
  if (!(ads_tbl.ads_config->_gain & ADS_CONF_GAIN)) gain = 0;
  switch (gain)
  {
    default:  //  catch invalid values and go for the safest gain.
    case 0:  ads_tbl.ads_config->_gain = ADS1X15_PGA_6_144V;  break;
    case 1:  ads_tbl.ads_config->_gain = ADS1X15_PGA_4_096V;  break;
    case 2:  ads_tbl.ads_config->_gain = ADS1X15_PGA_2_048V;  break;
    case 4:  ads_tbl.ads_config->_gain = ADS1X15_PGA_1_024V;  break;
    case 8:  ads_tbl.ads_config->_gain = ADS1X15_PGA_0_512V;  break;
    case 16: ads_tbl.ads_config->_gain = ADS1X15_PGA_0_256V;  break;
  }
}

void setDataRate(uint8_t dataRate)
{
  ads_tbl.ads_config->_datarate = dataRate;
  if(ads_tbl.ads_config->_datarate > 7) ads_tbl.ads_config->_datarate = 4;  // default
  ads_tbl.ads_config->_datarate <<= 5;
}

void setMode(uint8_t mode)
{
  switch (mode)
  {
    case 0: ads_tbl.ads_config->_mode = ADS1X15_MODE_CONTINUE; break;
    default:  //  catch invalid modi
    case 1: ads_tbl.ads_config->_mode = ADS1X15_MODE_SINGLE;   break;
  }
}

void _requestADC(uint16_t readmode)
{
  //  write to register is needed in continuous mode as other flags can be changed
  uint16_t config = ADS1X15_OS_START_SINGLE;  //  bit 15     force wake up if needed
  config |= readmode;                         //  bit 12-14
  config |= ads_tbl.ads_config->_gain;                            //  bit 9-11
  config |= ads_tbl.ads_config->_mode;                            //  bit 8
  config |= ads_tbl.ads_config->_datarate;                        //  bit 5-7
  if (ads_tbl.ads_comp->_compMode)  config |= ADS1X15_COMP_MODE_WINDOW;         //  bit 4      comparator modi
  else                              config |= ADS1X15_COMP_MODE_TRADITIONAL;
  if (ads_tbl.ads_comp->_compPol)   config |= ADS1X15_COMP_POL_ACTIV_HIGH;      //  bit 3      ALERT active value
  else                              config |= ADS1X15_COMP_POL_ACTIV_LOW;
  if (ads_tbl.ads_comp->_compLatch) config |= ADS1X15_COMP_LATCH;
  else                              config |= ADS1X15_COMP_NON_LATCH;           //  bit 2      ALERT latching
  config |= ads_tbl.ads_comp->_compQueConvert;                                  //  bit 0..1   ALERT mode
   
  uint8_t config_data[2] = {config >> 8, config & 0x00FF};
  i2cWriteBytes(ads_tbl._ch, ads_tbl.ads_config->_address, ADS1X15_REG_CONFIG, config_data, sizeof(config_data), 100);
  //  remember last request type.
  config |= ads_tbl.ads_config->_lastRequest = readmode;
}

// i2cWriteBytes(_DEF_I2C1, 0x48, 0x01, i2c_request, sizeof(i2c_request), 100);
// i2cReadBytes(_DEF_I2C1, 0x48, 0x00, i2c_data, 2, 100);

bool isReady()
{
  uint8_t i2c_data[4] = {0,};
  if (i2cReadByte(ads_tbl._ch, ads_tbl.ads_config->_address, ADS1X15_REG_CONFIG, i2c_data, 100)) return false;

  uint16_t val = i2c_data[0] << 8 |  i2c_data[1];
  
  return ((val & ADS1X15_OS_NOT_BUSY) > 0);
}

bool isBusy()
{
  return isReady() == false;
}

int16_t _readADC(uint16_t readmode)
{
  //  note readmode includes the channel
  _requestADC(readmode);

  if (ads_tbl.ads_config->_mode == ADS1X15_MODE_SINGLE)
  {
    uint32_t start = millis();
    uint8_t timeOut = (128 >> (ads_tbl.ads_config->_datarate >> 5)) + 10;
    while (isBusy())
    {
      if ( (millis() - start) > timeOut)
      {
        ads_tbl._error = ADS1X15_ERROR_TIMEOUT;
        return ADS1X15_ERROR_TIMEOUT;
      }
      // yield();   //  wait for conversion; yield for ESP.
    }
  }
  else
  {
    //  needed in continuous mode too, otherwise one get an old value.
    delay(ads_tbl.ads_config->_conversionDelay);
  }

  return getValue();
}

uint16_t readADC(uint8_t pin)
{
  if (pin >= ads_tbl.ads_config->_maxPorts) return 0;
  uint16_t mode = ((4 + pin) << 12);  //  pin to mask
  return _readADC(mode);
} 


uint16_t getValue()
{
  uint8_t i2c_data[2];
  i2cReadBytes(ads_tbl._ch, ads_tbl.ads_config->_address, ADS1X15_REG_CONVERT, i2c_data, sizeof(i2c_data), 100);
  int16_t raw = i2c_data[0] << 8 | i2c_data[1];

  if(ads_tbl.ads_config->_bitShift) raw >>= ads_tbl.ads_config->_bitShift;

  ads1115_packet.ain_data.ain0 = raw;

  return raw;
}



