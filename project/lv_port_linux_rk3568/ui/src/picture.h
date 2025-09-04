#ifndef __PICTURE_H__
#define __PICTURE_H__

#include "../ui.h"

/* ================== 图片 =================*/
// 图片资源列表
static const lv_img_dsc_t* image_list[] = {
    &ui_img_car1_png,
    &ui_img_car2_png,
    &ui_img_car3_png
    // 可以继续添加更多图片
};

static const uint8_t image_count = sizeof(image_list) / sizeof(image_list[0]);
static uint8_t current_image_index = 0; // 当前图片索引
static lv_obj_t* ui_current_image = NULL; // 当前显示的图片对象
static lv_obj_t* page_label = NULL; // 页码标签对象

// 添加全局函数声明（在文件顶部）
void init_picture();
void screen_gesture_handler(lv_event_t *e);
void switch_image(bool next);
void image_navigation_handler(lv_event_t *e);

#endif