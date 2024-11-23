/*********************
 *      INCLUDES
 *********************/
#include "lv_port_indev.h"
#include "lcd.h"
#include "button.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static int8_t button_get_pressed_id(void);
static void button_read(lv_indev_t * indev, lv_indev_data_t * data);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_indev_t * indev_button;


/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_indev_init(void)
{

  /*------------------
   * Keypad
   * -----------------*/

  /*Register a button input device*/
  indev_button = lv_indev_create();
  lv_indev_set_type(indev_button, LV_INDEV_TYPE_BUTTON);
  lv_indev_set_read_cb(indev_button, button_read);

  /*Assign buttons to points on the screen*/
  static const lv_point_t btn_points[1] = {
      {LCD_WIDTH/2, LCD_HEIGHT/2},
  };
  lv_indev_set_button_points(indev_button, btn_points);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


/*------------------
 * Button
 * -----------------*/


/*Will be called by the library to read the button*/
static void button_read(lv_indev_t * indev_drv, lv_indev_data_t * data)
{
  static uint8_t last_btn = 0;

  /*Get the pressed button's ID*/
  int8_t btn_act = button_get_pressed_id();

  if(btn_act >= 0) {
      data->state = LV_INDEV_STATE_PRESSED;
      last_btn = 0;
      logPrintf("pressed\n");
  }
  else {
      data->state = LV_INDEV_STATE_RELEASED;
  }

  /*Save the last pressed button's ID*/
  data->btn_id = last_btn;
}

/*Get ID  (0, 1, 2 ..) of the pressed button*/
static int8_t button_get_pressed_id(void)
{
  uint8_t i;

  /*Check to buttons see which is being pressed (assume there are 2 buttons)*/
  for(i = 0; i < BUTTON_MAX_CH; i++)
  {
    /*Return the pressed button's ID*/
    if(buttonGetPressed(i))
    {
      return i;
    }
  }

  /*No button pressed*/
  return -1;
}
