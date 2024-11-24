#include "test.h"


static void appInit(void);
static void appMain(void);

static app_info_t app_info = {
    .name = "TEST",
    .init = appInit,
    .run_func = appMain,
};


app_info_t *testGetAppInfo(void)
{
  return &app_info;
}

void appInit(void)
{

}

void appMain(void)
{
  while(1)
  {
    if (buttonGetPressed(0))
    {
      break;
    }

    if (lcdDrawAvailable())
    {
      static uint32_t pre_time_draw;
      static uint32_t exe_time_draw;


      pre_time_draw = millis();

      lcdClearBuffer(black);
      exe_time_draw = millis() - pre_time_draw;

      lcdPrintf(5, 5, white, "테스트");
      lcdPrintf(5, 25, white, "%d ms", exe_time_draw);
      lcdPrintf(5, 45, white, "%d ms", lcdGetDrawTime());
      lcdPrintf(150, 5, white, "X %d", adcRead(0));
      lcdPrintf(150, 25, white, "Y %d", adcRead(1));

      for (int i=0; i<BUTTON_MAX_CH; i++)
      {
        lcdPrintf(5+16*i, 65, white, "%d", buttonGetPressed(i));
      }

      lcdRequestDraw();
    }
  }
}
