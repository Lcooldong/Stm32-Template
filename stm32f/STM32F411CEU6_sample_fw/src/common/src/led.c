#include "led.h"

#ifdef _USE_HW_LED

typedef struct 
{
  GPIO_TypeDef *port;
  uint16_t      pin;
  GPIO_PinState on_state;
  GPIO_PinState off_state;
} led_tbl_t;

const led_tbl_t led_tbl[LED_MAX_CH] = 
{
  {GPIOC, GPIO_PIN_13,  GPIO_PIN_RESET, GPIO_PIN_SET},   // 0. DEBUG
};

bool ledInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  for (uint32_t i = 0; i < LED_MAX_CH; i++)
  {
    GPIO_InitStruct.Pin = led_tbl[i].pin;
    HAL_GPIO_Init(led_tbl[i].port, &GPIO_InitStruct);
  }
  

  

  return true;
}


void ledOn(uint8_t ch)
{
  if (ch >= LED_MAX_CH) return;

  HAL_GPIO_WritePin(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].on_state);
}

void ledOff(uint8_t ch)
{
  if (ch >= LED_MAX_CH) return;

  HAL_GPIO_WritePin(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].off_state);
}

void ledToggle(uint8_t ch)
{
  if (ch >= LED_MAX_CH) return;

  HAL_GPIO_TogglePin(led_tbl[ch].port, led_tbl[ch].pin);
}

#ifdef _USE_HW_CLI
void cliLed(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "info"))
  {
    for (int i=0; i<LED_MAX_CH; i++)
    {
      cliPrintf("%-12s \n", led_name[i]);
    }
    ret = true;
  }

  if (args->argc == 3 && args->isStr(0, "toggle"))
  {
    uint8_t  ch;
    uint32_t toggle_time;
    uint32_t pre_time;

    ch = (uint8_t)args->getData(1);
    ch = constrain(ch, 0, LED_MAX_CH-1);
    toggle_time = (uint32_t)args->getData(2);

    pre_time = millis();
    while(cliKeepLoop())
    {
      if (millis()-pre_time >= toggle_time)
      {
        pre_time = millis();
        ledToggle(ch);
      }
    }
    ledOff(ch);
    ret = true;
  }

  if (ret == false)
  {
    cliPrintf("led info\n");
    cliPrintf("led toggle 0~%d ms\n", LED_MAX_CH);
  }
}

#endif

#endif