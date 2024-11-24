#include "launcher.h"

#include "app/game/game.h"
#include "app/test/test.h"


#define APP_MAX_CNT     8



static void uiInit(void);
static void uiEvent(lv_event_t * e);

extern lv_indev_t * indev_keypad;

static uint8_t app_cnt = 0;
static app_info_t *p_app_info[APP_MAX_CNT];
static bool is_req_run = false;
static uint8_t req_run_id = 0;
static lv_obj_t *main_disp = NULL;





bool launcherInit(void)
{
  p_app_info[app_cnt++] = gameGetAppInfo();
  p_app_info[app_cnt++] = testGetAppInfo();

  lvglInit();

  uiInit();
  return true;
}

void launcherUpdate(void)
{
  lvglUpdate();

  if (is_req_run)
  {
    is_req_run = false;
    p_app_info[req_run_id]->init();
    p_app_info[req_run_id]->run_func();

    lv_obj_invalidate(main_disp); 
  }
}

void uiEvent(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);

  logPrintf("key event  %d\n", code);
  logPrintf("id  %d\n", (int)lv_event_get_user_data(e));

  req_run_id = (int)lv_event_get_user_data(e);
  is_req_run = true;
}

void uiInit(void)
{
  main_disp = lv_obj_create(lv_screen_active());
  lv_obj_set_size(main_disp, LCD_WIDTH, LCD_HEIGHT);
  lv_obj_set_scroll_snap_x(main_disp, LV_SCROLL_SNAP_CENTER);
  lv_obj_set_scroll_snap_y(main_disp, LV_SCROLL_SNAP_CENTER);
  lv_obj_set_flex_flow(main_disp, LV_FLEX_FLOW_ROW);
  lv_obj_align(main_disp, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_scrollbar_mode(main_disp, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_scroll_dir(main_disp, LV_DIR_HOR);


  lv_group_t * g = lv_group_create();

  for (int i = 0; i < app_cnt; i++)
  {
    lv_obj_t *btn = lv_button_create(main_disp);
    lv_obj_set_size(btn, 150, lv_pct(50));

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text_fmt(label, p_app_info[i]->name);
    lv_obj_center(label);
    lv_group_add_obj(g, btn);

    lv_obj_add_event_cb(btn, uiEvent, LV_EVENT_CLICKED, (void *)i);
  }
  lv_indev_set_group(indev_keypad, g);

  lv_obj_scroll_to_view(lv_obj_get_child(main_disp, 0), LV_ANIM_OFF);
  lv_group_focus_obj(lv_obj_get_child(main_disp, 0));
  lv_obj_update_snap(main_disp, LV_ANIM_ON);
}
