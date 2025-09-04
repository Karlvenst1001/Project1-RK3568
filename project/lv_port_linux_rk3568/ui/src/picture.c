#include "../src/picture.h"

/* ===================== 图片显示 =======================*/
void init_picture()
{
    // 1. 创建图片显示区域
    ui_current_image = lv_img_create(ui_Screen3);
    lv_img_set_src(ui_current_image, image_list[0]); // 显示第一张图片
    lv_obj_set_size(ui_current_image, 800, 450); // 设置显示尺寸
    lv_obj_align(ui_current_image, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(ui_current_image, LV_OBJ_FLAG_ADV_HITTEST);
    
    // 设置图片样式
    lv_obj_set_style_border_color(ui_current_image, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_border_width(ui_current_image, 5, 0);
    lv_obj_set_style_radius(ui_current_image, 10, 0);
    lv_obj_set_style_shadow_color(ui_current_image, lv_color_hex(0x000000), 0);
    lv_obj_set_style_shadow_opa(ui_current_image, 150, 0);
    lv_obj_set_style_shadow_width(ui_current_image, 15, 0);
    
    // 2. 创建页码标签
    page_label = lv_label_create(ui_Screen3);
    lv_label_set_text_fmt(page_label, "%d/%d", 1, image_count);
    lv_obj_align(page_label, LV_ALIGN_BOTTOM_MID, 0, -20);
    
    // 使用项目中存在的字体
    lv_obj_set_style_text_font(page_label, &lv_font_montserrat_20, 0); // 使用20号字体
    lv_obj_set_style_text_color(page_label, lv_color_hex(0xFFFFFF), 0);
    
    // 3. 绑定左右按钮事件
    lv_obj_add_event_cb(ui_leftbtn, image_navigation_handler, LV_EVENT_CLICKED, (void*)(intptr_t)0);
    lv_obj_add_event_cb(ui_rightbtn, image_navigation_handler, LV_EVENT_CLICKED, (void*)(intptr_t)1);
    
    // 4. 添加手势支持
    lv_obj_add_event_cb(ui_Screen3, screen_gesture_handler, LV_EVENT_ALL, NULL);
}


void switch_image(bool next) {
    // 更新索引
    if(next) {
        current_image_index = (current_image_index + 1) % image_count;
    } else {
        current_image_index = (current_image_index == 0) ? 
                             (image_count - 1) : 
                             (current_image_index - 1);
    }
    
    // 更新图片显示
    lv_img_set_src(ui_current_image, image_list[current_image_index]);
    
    // 添加切换动画
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, ui_current_image);
    lv_anim_set_values(&anim, next ? 800 : -800, 0); // 从右或左滑入
    lv_anim_set_time(&anim, 300); // 300毫秒动画
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_x);
    lv_anim_start(&anim);
    
    // 更新页码标签
    lv_label_set_text_fmt(page_label, "%d/%d", 
                         current_image_index + 1, 
                         image_count);
}


void image_navigation_handler(lv_event_t *e) {
    if(e->code == LV_EVENT_CLICKED) {
        // 从用户数据获取方向 (0:上一张, 1:下一张)
        int direction = (int)(intptr_t)lv_event_get_user_data(e);
        
        // 切换图片 (true:下一张, false:上一张)
        switch_image(direction == 1);
    }
}


// 添加手势处理函数实现
void screen_gesture_handler(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *screen = lv_event_get_target(e);
    lv_indev_t *indev = lv_indev_get_act();
    lv_point_t point;
    lv_indev_get_point(indev, &point);
    
    static lv_point_t start_point;
    static uint32_t start_time = 0;
    
    if(code == LV_EVENT_PRESSED) {
        start_point = point;
        start_time = lv_tick_get();
    }
    else if(code == LV_EVENT_RELEASED) {
        // 计算滑动距离和时间
        int16_t dx = point.x - start_point.x;
        uint32_t dt = lv_tick_elaps(start_time);
        
        // 检测有效滑动（距离>50px且时间<300ms）
        if(abs(dx) > 50 && dt < 300) {
            if(dx > 0) {
                switch_image(false); // 向右滑显示上一张
            } else {
                switch_image(true); // 向左滑显示下一张
            }
        }
    }
}