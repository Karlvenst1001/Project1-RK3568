#include "gpio.h"

// 闪烁状态变量
static uint8_t left_turn_active = 0;
static uint8_t right_turn_active = 0;
static uint8_t hazard_active = 0;
static uint8_t blink_state = 0;

// 按钮事件处理函数
void ui_event_turnleftbtn(lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    
    if(event_code == LV_EVENT_PRESSED) {
        // 切换左转灯状态
        left_turn_active = !left_turn_active;
        
        // 如果开启左转灯，关闭双闪和右转
        if (left_turn_active) {
            hazard_active = 0;
            right_turn_active = 0;
            
            // 更新UI
            lv_obj_set_style_img_recolor(ui_turnleft, lv_color_hex(0xFFA500), 0);
            lv_obj_set_style_img_recolor(ui_turnright, lv_color_hex(0xFFFFFF), 0);
            lv_obj_set_style_img_recolor(ui_warning, lv_color_hex(0xFFFFFF), 0);
        } else {
            // 关闭左转灯
            set_gpio_value(LEFT_TURN_GPIO, 0);
            lv_obj_set_style_img_recolor_opa(ui_turnleft, LV_OPA_0, 0);
        }
    }
}

void ui_event_turnrightbtn(lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    
    if(event_code == LV_EVENT_PRESSED) {
        // 切换右转灯状态
        right_turn_active = !right_turn_active;
        
        // 如果开启右转灯，关闭双闪和左转
        if (right_turn_active) {
            hazard_active = 0;
            left_turn_active = 0;
            
            // 更新UI
            lv_obj_set_style_img_recolor(ui_turnright, lv_color_hex(0xFFA500), 0);
            lv_obj_set_style_img_recolor(ui_turnleft, lv_color_hex(0xFFFFFF), 0);
            lv_obj_set_style_img_recolor(ui_warning, lv_color_hex(0xFFFFFF), 0);
        } else {
            // 关闭右转灯
            set_gpio_value(RIGHT_TURN_GPIO, 0);
            lv_obj_set_style_img_recolor_opa(ui_turnright, LV_OPA_0, 0);
        }
    }
}

void ui_event_warningbtn(lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    
    if(event_code == LV_EVENT_PRESSED) {
        // 切换双闪状态
        hazard_active = !hazard_active;
        
        // 如果开启双闪，关闭左右转向灯
        if (hazard_active) {
            left_turn_active = 0;
            right_turn_active = 0;
            
            // 更新UI
            lv_obj_set_style_img_recolor(ui_turnleft, lv_color_hex(0xFFA500), 0);
            lv_obj_set_style_img_recolor(ui_turnright, lv_color_hex(0xFFA500), 0);
            lv_obj_set_style_img_recolor(ui_warning, lv_color_hex(0xFF0000), 0);
        } else {
            // 关闭双闪灯
            set_gpio_value(HAZARD_LEFT_GPIO, 0);
            set_gpio_value(HAZARD_RIGHT_GPIO, 0);
            
            lv_obj_set_style_img_recolor_opa(ui_turnleft, LV_OPA_0, 0);
            lv_obj_set_style_img_recolor_opa(ui_turnright, LV_OPA_0, 0);
            lv_obj_set_style_img_recolor_opa(ui_warning, LV_OPA_0, 0);
        }
    }
}

void ui_event_lababtn(lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    
    if(event_code == LV_EVENT_PRESSED) {
        // 切换蜂鸣器状态
        static uint8_t buzzer_state = 0;
        buzzer_state = !buzzer_state;
        
        // 更新GPIO
        set_gpio_value(BUZZER_GPIO, buzzer_state);
        
        // 更新UI反馈
        if (buzzer_state) {
            lv_obj_set_style_img_recolor(ui_laba, lv_color_hex(0x00FF00), 0);
            lv_obj_set_style_img_recolor_opa(ui_laba, LV_OPA_COVER, 0);
        } else {
            lv_obj_set_style_img_recolor(ui_laba, lv_color_hex(0xFFFFFF), 0);
            lv_obj_set_style_img_recolor_opa(ui_laba, LV_OPA_0, 0);
        }
        
        // 添加简单动画
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, ui_laba);
        lv_anim_set_values(&a, LV_IMG_ZOOM_NONE, LV_IMG_ZOOM_NONE * 1.5);
        lv_anim_set_time(&a, 200);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_img_set_zoom);
        lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
        lv_anim_set_playback_time(&a, 200);
        lv_anim_start(&a);
    }
}


// GPIO控制函数
void gpio_export(int gpio) {
    char cmd[100];
    snprintf(cmd, sizeof(cmd), "echo %d > /sys/class/gpio/export", gpio);
    system(cmd);
}

void set_gpio_direction(int gpio, const char *direction) {
    char cmd[100];
    snprintf(cmd, sizeof(cmd), "echo %s > /sys/class/gpio/gpio%d/direction", direction, gpio);
    system(cmd);
}

void set_gpio_value(int gpio, int value) {
    char cmd[100];
    snprintf(cmd, sizeof(cmd), "echo %d > /sys/class/gpio/gpio%d/value", value, gpio);
    system(cmd);
}

// 初始化所有GPIO
void init_gpio(void) {
    // 导出并初始化左转向灯GPIO
    if (access("/sys/class/gpio/gpio120", F_OK) == -1) {
        gpio_export(LEFT_TURN_GPIO);
    }
    set_gpio_direction(LEFT_TURN_GPIO, "out");
    set_gpio_value(LEFT_TURN_GPIO, 0);
    
    // 导出并初始化右转向灯GPIO
    if (access("/sys/class/gpio/gpio121", F_OK) == -1) {
        gpio_export(RIGHT_TURN_GPIO);
    }
    set_gpio_direction(RIGHT_TURN_GPIO, "out");
    set_gpio_value(RIGHT_TURN_GPIO, 0);
    
    // 导出并初始化双闪左灯GPIO
    if (access("/sys/class/gpio/gpio121", F_OK) == -1) {
        gpio_export(HAZARD_LEFT_GPIO);
    }
    set_gpio_direction(HAZARD_LEFT_GPIO, "out");
    set_gpio_value(HAZARD_LEFT_GPIO, 0);
    
    // 导出并初始化双闪右灯GPIO
    if (access("/sys/class/gpio/gpio123", F_OK) == -1) {
        gpio_export(HAZARD_RIGHT_GPIO);
    }
    set_gpio_direction(HAZARD_RIGHT_GPIO, "out");
    set_gpio_value(HAZARD_RIGHT_GPIO, 0);

    // 导出并初始化蜂鸣器GPIO
    if (access("/sys/class/gpio/gpio111", F_OK) == -1) {
        gpio_export(BUZZER_GPIO);
    }
    set_gpio_direction(BUZZER_GPIO, "out");
    set_gpio_value(BUZZER_GPIO, 0);
}


// 闪烁定时器回调函数
void blink_timer_callback(lv_timer_t * timer) {
    // 切换闪烁状态
    blink_state = !blink_state;
    
    // 更新左转向灯
    if (left_turn_active && !hazard_active) {
        set_gpio_value(LEFT_TURN_GPIO, blink_state);
        lv_obj_set_style_img_recolor_opa(ui_turnleft, blink_state ? LV_OPA_COVER : LV_OPA_50, 0);
    }
    
    // 更新右转向灯
    if (right_turn_active && !hazard_active) {
        set_gpio_value(RIGHT_TURN_GPIO, blink_state);
        lv_obj_set_style_img_recolor_opa(ui_turnright, blink_state ? LV_OPA_COVER : LV_OPA_50, 0);
    }
    
    // 更新双闪灯
    if (hazard_active) {
        set_gpio_value(HAZARD_LEFT_GPIO, blink_state);
        set_gpio_value(HAZARD_RIGHT_GPIO, blink_state);
        
        lv_obj_set_style_img_recolor_opa(ui_turnleft, blink_state ? LV_OPA_COVER : LV_OPA_50, 0);
        lv_obj_set_style_img_recolor_opa(ui_turnright, blink_state ? LV_OPA_COVER : LV_OPA_50, 0);
        lv_obj_set_style_img_recolor_opa(ui_warning, blink_state ? LV_OPA_COVER : LV_OPA_50, 0);
    }
}