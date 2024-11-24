#include "game.h"


static void appInit(void);
static void appMain(void);

static app_info_t app_info = {
    .name = "GAME",
    .init = appInit,
    .run_func = appMain,
};

LVGL_IMG_DEF(fighter);
LVGL_IMG_DEF(bullet);
LVGL_IMG_DEF(bomb);


app_info_t *gameGetAppInfo(void)
{
  return &app_info;
}

void appInit(void)
{

}

void appMain(void)
{
  typedef struct
  {
    bool is_enable;
    uint8_t type;
    uint16_t color;
    int16_t x;
    int16_t y;
    int16_t size;
    int16_t speed;
  } bomb_info_t;


  int16_t block_x = 0;
  int16_t block_y = 0;
  int16_t block_size = 20;
  uint16_t block_speed = 4;
  uint32_t pre_time_buzzer;
  bomb_info_t bomb_info[10];

  image_t img_fighter;
  image_t img_bullet;
  image_t img_bomb;


  for (int i = 0; i < 10; i++)
  {
    bomb_info[i].is_enable = false;
  }

  img_fighter = lcdCreateImage(&fighter, 0, 0, 0, 0);
  img_bullet = lcdCreateImage(&bullet, 0, 0, 0, 0);
  img_bomb = lcdCreateImage(&bomb, 0, 0, 0, 0);


  while(1)
  {
    if (buttonGetPressed(0))
    {
      break;
    }

    if (lcdDrawAvailable())
    {
      lcdClearBuffer(black);

      lcdDrawImage(&img_fighter, block_x, block_y);


      lcdDrawRect(0, 0, LCD_WIDTH, LCD_HEIGHT, white);

      if (adcRead(0) < 1500)
      {
        block_x = constrain((block_x - block_speed), 0, LCD_WIDTH);
      }
      if (adcRead(0) > 2500)
      {
        block_x = constrain((block_x + block_speed), 0, LCD_WIDTH);
      }
      if (adcRead(1) > 2500)
      {
        block_y = constrain((block_y - block_speed), 0, LCD_HEIGHT);
      }
      if (adcRead(1) < 1500)
      {
        block_y = constrain((block_y + block_speed), 0, LCD_HEIGHT);
      }


      for (int i=0; i<10; i++)
      {
        if (bomb_info[i].is_enable)
        {
          bomb_info[i].x += bomb_info[i].speed;
          if (bomb_info[i].x > LCD_WIDTH)
          {
            bomb_info[i].is_enable = false;
          }

          if (bomb_info[i].type == 0)
          {
            lcdDrawImage(&img_bullet, bomb_info[i].x, bomb_info[i].y);
          }
          if (bomb_info[i].type == 1)
          {
            lcdDrawImage(&img_bomb, bomb_info[i].x, bomb_info[i].y);
          }
        }
      }


      lcdRequestDraw();
    }

    if (buttonGetPressed(2) || buttonGetPressed(3))
    {
      if (millis()-pre_time_buzzer >= 200)
      {
        buzzerOn(1000, 100);
        pre_time_buzzer = millis();

        for (int i=0; i<10; i++)
        {
          if (!bomb_info[i].is_enable)
          {
            bomb_info[i].is_enable = true;
            bomb_info[i].y = block_y;
            bomb_info[i].x = block_x;
            bomb_info[i].size = block_size;

            if (buttonGetPressed(2))
            {
              bomb_info[i].type = 0;
              bomb_info[i].color = red;
              bomb_info[i].speed = block_speed * 3;
            }
            else
            {
              bomb_info[i].type = 1;
              bomb_info[i].color = blue;
              bomb_info[i].speed = block_speed;
            }
            break;
          }
        }
      }
    }
  }
}
