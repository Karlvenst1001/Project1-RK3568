#ifndef __GPIO_H__
#define __GPIO_H__
#include "../ui.h"

// 定义GPIO控制命令宏
#define EXPORT_GPIO(gpio) "echo " #gpio " > /sys/class/gpio/export"
#define UNEXPORT_GPIO(gpio) "echo " #gpio " > /sys/class/gpio/unexport"
#define SET_DIR_OUT(gpio) "echo out > /sys/class/gpio/gpio" #gpio "/direction"
#define SET_GPIO_HIGH(gpio) "echo 1 > /sys/class/gpio/gpio" #gpio "/value"
#define SET_GPIO_LOW(gpio) "echo 0 > /sys/class/gpio/gpio" #gpio "/value"

// 定义GPIO引脚（根据您的需求）
#define LEFT_TURN_GPIO 120
#define RIGHT_TURN_GPIO 124
#define HAZARD_LEFT_GPIO 121
#define HAZARD_RIGHT_GPIO 123
#define BUZZER_GPIO 111

// 事件函数
void ui_event_turnleftbtn(lv_event_t * e);
void ui_event_turnrightbtn(lv_event_t * e);
void ui_event_warningbtn(lv_event_t * e);
void ui_event_lababtn(lv_event_t * e);

// gpio函数
void gpio_export(int gpio);
void set_gpio_direction(int gpio, const char *direction);
void set_gpio_value(int gpio, int value);
void init_gpio(void);
void blink_timer_callback(lv_timer_t * timer);


#endif