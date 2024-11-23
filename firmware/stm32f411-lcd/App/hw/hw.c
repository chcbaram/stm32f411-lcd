#include "hw.h"




bool hwInit(void)
{
  cliInit();
  ledInit();
  uartInit();
  buttonInit();
  buzzerInit();
  adcInit();
  gpioInit();
  st7789Init();
  lcdInit();

  return true;
}

void delay(uint32_t ms)
{
  HAL_Delay(ms);
}

uint32_t millis(void)
{
  return HAL_GetTick();
}
