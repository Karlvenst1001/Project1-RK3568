#ifndef __TIMER_H__
#define __TIMER_H__

#include "../ui.h"

// 定义北京时区偏移（UTC+8）
#define BEIJING_TIME_OFFSET (8 * 3600)

// 在文件顶部添加声明
static lv_obj_t *ui_time_label = NULL;
static lv_obj_t *ui_timezone_label = NULL;
static time_t get_beijing_time(void);


void update_beijing_time(lv_timer_t *timer);
void init_calendar();
void init_timer();

#endif