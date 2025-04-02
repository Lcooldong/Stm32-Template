#include "i2c.h"

#ifdef _USE_HW_I2C

#ifdef _USE_HW_RTOS
#define lock_scan(x) osMutexWait(mutex_lock[x], 100)
#define lock(x) osMutexWait(mutex_lock[x], osWaitForever)
//  #define LOCK_BEGIN(x) osMutexWait(mutex_lock[x], 100)
#define unLock(x)   osMutexRelease(mutex_lock[x])
#else
#define lock(x)
#define unLock(x)
#endif


static uint32_t i2cGetTimming(uint32_t freq_khz);
static void delayUs(uint32_t us);


static uint32_t i2c_timeout[I2C_MAX_CH];
static uint32_t i2c_errcount[I2C_MAX_CH];
static uint32_t i2c_freq[I2C_MAX_CH];

static bool is_init = false;
static bool is_begin[I2C_MAX_CH];
#ifdef _USE_HW_RTOS
static SemaphoreHandle_t mutex_lock;
#endif


extern I2C_HandleTypeDef hi2c1;

typedef struct
{
  I2C_TypeDef       *p_i2c;
  I2C_HandleTypeDef *p_hi2c;

  GPIO_TypeDef *scl_port;
  int           scl_pin;

  GPIO_TypeDef *sda_port;
  int           sda_pin;
} i2c_tbl_t;


static i2c_tbl_t i2c_tbl[I2C_MAX_CH] =
{
    {I2C1, &hi2c1, I2C_SCL_GPIO_Port, I2C_SCL_Pin,  I2C_SDA_GPIO_Port, I2C_SDA_Pin},
};

bool i2cInit(void)
{
  uint32_t i;

#ifdef _USE_HW_RTOS
  mutex_lock = xSemaphoreCreateMutex();
#endif

  for (i=0; i<I2C_MAX_CH; i++)
  {
    i2c_timeout[i] = 10;
    i2c_errcount[i] = 0;
    is_begin[i] = false;
  }

// #if CLI_USE(HW_I2C)
//   cliAdd("i2c", cliI2C);
// #endif

  is_init = true;
  return true;
}

bool i2cIsInit(void)
{
  return is_init;
}

bool i2cBegin(uint8_t ch, uint32_t freq_khz)
{
  bool ret = false;

  I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

  if (ch >= I2C_MAX_CH)
  {
    return false;
  }


  switch(ch)
  {
    case _DEF_I2C1:
    case _DEF_I2C2:
      i2c_freq[ch] = freq_khz;

      p_handle->Instance             = i2c_tbl[ch].p_i2c;
      p_handle->Init.Timing          = i2cGetTimming(freq_khz);
      p_handle->Init.OwnAddress1     = 0x00;
      p_handle->Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
      p_handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
      p_handle->Init.OwnAddress2     = 0x00;
      p_handle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
      p_handle->Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

      i2cReset(ch);

      HAL_I2C_DeInit(p_handle);
      if(HAL_I2C_Init(p_handle) != HAL_OK)
      {
      }

      /* Enable the Analog I2C Filter */
      HAL_I2CEx_ConfigAnalogFilter(p_handle,I2C_ANALOGFILTER_ENABLE);

      /* Configure Digital filter */
      HAL_I2CEx_ConfigDigitalFilter(p_handle, 0);

      ret = true;
      is_begin[ch] = true;
      break;
  }

  return ret;
}

uint32_t i2cGetTimming(uint32_t freq_khz)
{
  uint32_t ret;

  switch(freq_khz)
  {
    case 100:
      ret = 0x20C0EDFF;
      break;

    case 400:
      ret = 0x00E063FF;
      break;

    default:
      ret = 0x00E063FF;
      break;
  };

  return ret;
}

bool i2cIsBegin(uint8_t ch)
{
  return is_begin[ch];
}

void i2cReset(uint8_t ch)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  i2c_tbl_t *p_pin = &i2c_tbl[ch];

  lock();
  GPIO_InitStruct.Pin       = p_pin->scl_pin;
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(p_pin->scl_port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = p_pin->sda_pin;
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init(p_pin->sda_port, &GPIO_InitStruct);


  HAL_GPIO_WritePin(p_pin->scl_port, p_pin->scl_pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(p_pin->sda_port, p_pin->sda_pin, GPIO_PIN_SET);
  delayUs(5);

  for (int i = 0; i < 9; i++)
  {

    HAL_GPIO_WritePin(p_pin->scl_port, p_pin->scl_pin, GPIO_PIN_RESET);
    delayUs(5);
    HAL_GPIO_WritePin(p_pin->scl_port, p_pin->scl_pin, GPIO_PIN_SET);
    delayUs(5);
  }

  HAL_GPIO_WritePin(p_pin->scl_port, p_pin->scl_pin, GPIO_PIN_RESET);
  delayUs(5);
  HAL_GPIO_WritePin(p_pin->sda_port, p_pin->sda_pin, GPIO_PIN_RESET);
  delayUs(5);

  HAL_GPIO_WritePin(p_pin->scl_port, p_pin->scl_pin, GPIO_PIN_SET);
  delayUs(5);
  HAL_GPIO_WritePin(p_pin->sda_port, p_pin->sda_pin, GPIO_PIN_SET);
  unLock();
}

bool i2cIsDeviceReady(uint8_t ch, uint8_t dev_addr)
{
  bool ret = false;
  I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

  lock_scan();
  if (HAL_I2C_IsDeviceReady(p_handle, dev_addr << 1, 10, 10) == HAL_OK)
  {
    __enable_irq();
    ret = true;
  }
  unLock();

  return ret;
}

bool i2cRecovery(uint8_t ch)
{
  bool ret;

  i2cReset(ch);

  ret = i2cBegin(ch, i2c_freq[ch]);

  return ret;
}

bool i2cReadByte (uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t timeout)
{
  return i2cReadBytes(ch, dev_addr, reg_addr, p_data, 1, timeout);
}

bool i2cReadBytes(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout)
{
  bool ret;
  HAL_StatusTypeDef i2c_ret;
  I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

  if (ch >= I2C_MAX_CH)
  {
    return false;
  }

  lock();
  i2c_ret = HAL_I2C_Mem_Read(p_handle, (uint16_t)(dev_addr << 1), reg_addr, I2C_MEMADD_SIZE_8BIT, p_data, length, timeout);
  unLock();

  if( i2c_ret == HAL_OK )
  {
    ret = true;
  }
  else
  {
    ret = false;
  }

  return ret;
}

bool i2cReadA16Bytes(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout)
{
  bool ret;
  HAL_StatusTypeDef i2c_ret;
  I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

  if (ch >= I2C_MAX_CH)
  {
    return false;
  }

  lock();
  i2c_ret = HAL_I2C_Mem_Read(p_handle, (uint16_t)(dev_addr << 1), reg_addr, I2C_MEMADD_SIZE_16BIT, p_data, length, timeout);
  unLock();

  if( i2c_ret == HAL_OK )
  {
    ret = true;
  }
  else
  {
    ret = false;
  }

  return ret;
}

bool i2cReadData(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint32_t length, uint32_t timeout)
{
  bool ret;
  HAL_StatusTypeDef i2c_ret;
  I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

  if (ch >= I2C_MAX_CH)
  {
    return false;
  }

  lock();
  i2c_ret = HAL_I2C_Master_Receive(p_handle, (uint16_t)(dev_addr << 1), p_data, length, timeout);
  unLock();

  if( i2c_ret == HAL_OK )
  {
    ret = true;
  }
  else
  {
    ret = false;
  }

  return ret;
}

bool i2cWriteByte (uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t data, uint32_t timeout)
{
  return i2cWriteBytes(ch, dev_addr, reg_addr, &data, 1, timeout);
}

bool i2cWriteBytes(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout)
{
  bool ret;
  HAL_StatusTypeDef i2c_ret;
  I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

  if (ch >= I2C_MAX_CH)
  {
    return false;
  }

  lock();
  i2c_ret = HAL_I2C_Mem_Write(p_handle, (uint16_t)(dev_addr << 1), reg_addr, I2C_MEMADD_SIZE_8BIT, p_data, length, timeout);
  unLock();

  if(i2c_ret == HAL_OK)
  {
    ret = true;
  }
  else
  {
    ret = false;
  }

  return ret;
}

bool i2cWriteA16Bytes(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout)
{
  bool ret;
  HAL_StatusTypeDef i2c_ret;
  I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

  if (ch >= I2C_MAX_CH)
  {
    return false;
  }

  lock();
  i2c_ret = HAL_I2C_Mem_Write(p_handle, (uint16_t)(dev_addr << 1), reg_addr, I2C_MEMADD_SIZE_16BIT, p_data, length, timeout);
  unLock();

  if(i2c_ret == HAL_OK)
  {
    ret = true;
  }
  else
  {
    ret = false;
  }

  return ret;
}

bool i2cWriteData(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint32_t length, uint32_t timeout)
{
  bool ret;
  HAL_StatusTypeDef i2c_ret;
  I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

  if (ch >= I2C_MAX_CH)
  {
    return false;
  }

  lock();
  i2c_ret = HAL_I2C_Master_Transmit(p_handle, (uint16_t)(dev_addr << 1), p_data, length, timeout);
  unLock();
  
  if(i2c_ret == HAL_OK)
  {
    ret = true;
  }
  else
  {
    ret = false;
  }

  return ret;
}

void i2cSetTimeout(uint8_t ch, uint32_t timeout)
{
  i2c_timeout[ch] = timeout;
}

uint32_t i2cGetTimeout(uint8_t ch)
{
  return i2c_timeout[ch];
}

void i2cClearErrCount(uint8_t ch)
{
  i2c_errcount[ch] = 0;
}

uint32_t i2cGetErrCount(uint8_t ch)
{
  return i2c_errcount[ch];
}

void delayUs(uint32_t us)
{
  volatile uint32_t i;

  for (i=0; i<us*1000; i++)
  {

  }
}


void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
  UNUSED(hi2c);
}


#endif
