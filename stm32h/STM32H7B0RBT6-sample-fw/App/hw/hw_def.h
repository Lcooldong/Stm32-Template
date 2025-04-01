#ifndef HW_DEF_H_
#define HW_DEF_H_


#include "main.h"
#include "def.h"


#define _USE_HW_SD
#define _USE_HW_BUZZER


#define _USE_HW_LED
#define      HW_LED_MAX_CH          2

#define _USE_HW_UART
#define      HW_UART_MAX_CH         3
#define      HW_UART_CH_SWD         _DEF_UART1
#define      HW_UART_CH_CLI         _DEF_UART1
#define      HW_UART_CH_232         _DEF_UART2
#define      HW_UART_CH_485         _DEF_UART3

// #define _USE_HW_CLI
#define      HW_CLI_CMD_LIST_MAX    32
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_LINE_HIS_MAX    8
#define      HW_CLI_LINE_BUF_MAX    64

#define _USE_HW_I2C
#define      HW_I2C_MAX_CH          1
#define      HW_I2C_CH_EEPROM       _DEF_I2C1

#define _USE_HW_EEPROM
#define      HW_EEPROM_MAX_SIZE     (16*1024)

#define _USE_HW_GPIO
#define      HW_GPIO_MAX_CH         GPIO_PIN_MAX

#define _USE_HW_SPI
#define      HW_SPI_MAX_CH          1

#define _USE_HW_SPI_FLASH
#define      HW_SPI_FLASH_ADDR      0x91000000

#define _USE_HW_BUTTON
#define      HW_BUTTON_MAX_CH       1

#define _USE_HW_RTC
#define      HW_RTC_BOOT_MODE       RTC_BKP_DR3
#define      HW_RTC_RESET_BITS      RTC_BKP_DR4


// #define _USE_HW_CAN
#define      HW_CAN_FD              0
#define      HW_CAN_MAX_CH          2
#define      HW_CAN_MSG_RX_BUF_MAX  32


#define _USE_HW_USB
#define _USE_HW_CDC
#define      HW_USE_CDC             1
#define      HW_USE_MSC             0
#define HW_UART_CH_USB              0

//-- USE CLI
//
#define _USE_CLI_HW_I2C             1
#define _USE_CLI_HW_EEPROM          1
#define _USE_CLI_HW_SPI_FLASH       1
#define _USE_CLI_HW_BUTTON          1
#define _USE_CLI_HW_RTC             1
#define _USE_CLI_HW_BUZZER          1
#define _USE_CLI_HW_SD              1
#define _USE_CLI_HW_WS2812          1


typedef enum
{
  SPI_FLASH_CS,
  SD_CD,
  RS485_DIR,
  GPIO_PIN_MAX,
} GpioPinName_t;


#define logPrintf printf

void     delay(uint32_t ms);
uint32_t millis(void);


#endif