#include "../src/timer.h"

void init_calendar()
{
    ui_Calendar1 = lv_calendar_create(ui_Screen2);
    // 获取当前系统时间
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    // 设置日历的今天日期
    lv_calendar_set_today_date(ui_Calendar1, timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday);
    // 设置日历的显示日期为当前月份
    lv_calendar_set_showed_date(ui_Calendar1, timeinfo->tm_year + 1900, timeinfo->tm_mon + 1);
    
    // 高亮显示当前日期
    static lv_calendar_date_t highlighted_dates[1];
    highlighted_dates[0].year = timeinfo->tm_year + 1900;
    highlighted_dates[0].month = timeinfo->tm_mon + 1;
    highlighted_dates[0].day = timeinfo->tm_mday;

    lv_calendar_set_highlighted_dates(ui_Calendar1, highlighted_dates, 1);

    lv_obj_t * ui_Calendar1_header = lv_calendar_header_arrow_create(ui_Calendar1);
    lv_obj_set_width(ui_Calendar1, 320);
    lv_obj_set_height(ui_Calendar1, 320);
    lv_obj_set_align(ui_Calendar1, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_Calendar1, lv_color_hex(0xEBDAC7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Calendar1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Calendar1, lv_color_hex(0xF3BB9C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Calendar1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Calendar1, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void init_timer()
{
    // 创建时区标识标签
    ui_timezone_label = lv_label_create(ui_top_nagivator);
    lv_label_set_text(ui_timezone_label, "BEIJING");
    lv_obj_set_style_text_color(ui_timezone_label, lv_color_hex(0x707070), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_timezone_label, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(ui_timezone_label, LV_ALIGN_CENTER, -350, 0); // 时间标签下方

    // 创建时间显示标签
    ui_time_label = lv_label_create(ui_top_nagivator);
    lv_obj_set_width(ui_time_label, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_time_label, LV_SIZE_CONTENT);
    lv_obj_align(ui_time_label, LV_ALIGN_CENTER, 0, 0); // 在导航栏中居中
    
    // 设置时间标签样式
    lv_obj_set_style_text_color(ui_time_label, lv_color_hex(0x707070), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_time_label, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui_time_label, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // 初始更新时间显示
    update_beijing_time(NULL);

    // 创建定时器每秒更新时间
    static lv_timer_t *timer = NULL;
    if (!timer) {
        timer = lv_timer_create(update_beijing_time, 1000, NULL); // 每秒更新一次
        lv_timer_set_repeat_count(timer, -1); // 无限重复
    }
}


// 获取北京时间（UTC+8）
time_t get_beijing_time(void)
{
    // 方法1：使用本地时间（如果设备时区已设置为北京时间）
    // time_t now = time(NULL);
    // return now;
    
    // 方法2：手动计算北京时间（UTC时间 + 8小时）
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + BEIJING_TIME_OFFSET;
}

// 更新时间显示的函数
// 更新北京时间显示的函数
void update_beijing_time(lv_timer_t *timer)
{
    // 获取北京时间
    time_t beijing_time = get_beijing_time();
    struct tm *timeinfo = localtime(&beijing_time);
    
    // 格式化为"HH:MM:SS"
    char time_str[9]; // "HH:MM:SS" + null terminator
    snprintf(time_str, sizeof(time_str), "%02d:%02d:%02d", 
             timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    // 更新标签文本
    lv_label_set_text(ui_time_label, time_str);
    
    // 根据时间显示问候语
    static const char *greetings[] = {
        [0] = "Good Morning Sir",    // 0:00 - 5:00
        [1] = "Good Morning Sir",    // 5:00 - 11:00
        [2] = "Good Afternoon Sir",    // 11:00 - 13:00
        [3] = "Good Evening Sir",    // 13:00 - 18:00
        [4] = "Good Nignt Sir",    // 18:00 - 24:00
    };
    
    int hour = timeinfo->tm_hour;
    const char *greeting = hour < 5 ? greetings[0] : 
                          hour < 11 ? greetings[1] : 
                          hour < 13 ? greetings[2] : 
                          hour < 18 ? greetings[3] : greetings[4];
    
    // 更新时区标签显示问候语
    char greeting_text[20];

    snprintf(greeting_text, sizeof(greeting_text), "%s", greeting);
    lv_label_set_text(ui_timezone_label, greeting_text);
    
    // 根据不同时间段调整文字颜色
    lv_color_t text_color = lv_color_hex(0x707070);
    
    lv_obj_set_style_text_color(ui_time_label, text_color, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_timezone_label, text_color, LV_PART_MAIN | LV_STATE_DEFAULT); 
}


