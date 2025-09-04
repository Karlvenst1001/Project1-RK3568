#include "lv_test.h"
#include "login_register.h"


// 回调事件
static void my_event_cb_callback(lv_event_t *event)
{
    lv_obj_t *target_obj = lv_event_get_target(event);              // 获取触发事件的对象
    lv_event_code_t code = lv_event_get_code(event);                // 获取触发事件的事件代码
    lv_obj_t *usr_data = (lv_obj_t *)lv_event_get_user_data(event); // 获取触发事件的用户参数
    switch (code)
    {
    case LV_EVENT_PRESSED: // 被按下
        printf("obj LV_EVENT_PRESSED\r\n");
        lv_obj_set_style_bg_color(target_obj, lv_color_hex(0xffff00), LV_STATE_PRESSED);
        break;
    case LV_EVENT_RELEASED: // 松开
        printf("obj LV_EVENT_RELEASED\r\n");
        lv_obj_set_style_bg_color(target_obj, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
        break;
    case LV_EVENT_LONG_PRESSED: // 长按
        printf("obj LV_EVENT_LONG_PRESSED\r\n");
        if (usr_data != NULL)
        {
            lv_event_send(usr_data, LV_EVENT_SCROLL, NULL); // 软件触发LV_EVENT_VALUE_CHANGED事件（注意：触发的事件要对象支持，不然会段错误）
        }
        break;
    case LV_EVENT_SCROLL:
        printf("obj2 LV_EVENT_SCROLL\r\n");
        lv_obj_set_style_bg_color(target_obj, lv_color_hex(0x00ffff), LV_STATE_DEFAULT);
        break;
    default:
        break;
    }
} 

// 回调事件
void even_callback(lv_obj_t *screen)
{
    lv_obj_t *obj1 = lv_obj_create(screen); // 在活动窗口，创建一个对象
    lv_obj_t *obj2 = lv_obj_create(screen); // 在活动窗口，创建一个对象
    lv_obj_center(obj2);
    lv_obj_add_event_cb(obj1, my_event_cb_callback, LV_EVENT_ALL, obj2);    // 添加事件
    lv_obj_add_event_cb(obj2, my_event_cb_callback, LV_EVENT_SCROLL, NULL); // 添加事件
}


// 事件冒泡
static void my_event_cb_bubble(lv_event_t *e)
{
    lv_obj_t *target_obj = lv_event_get_target(e);
    lv_obj_t *parent = lv_event_get_current_target(e);
    lv_event_code_t code = lv_event_get_code(e);
    if (target_obj == parent)
    {
        if (code == LV_EVENT_PRESSED)
        {
            printf("child %p LV_EVENT_PRESSED\r\n", parent);
            lv_obj_set_style_bg_color(target_obj, lv_color_hex(0xffff00), LV_STATE_PRESSED);
        }
        else if(code == LV_EVENT_RELEASED)
        {
            printf("child %p LV_EVENT_RELEASED\r\n", parent);
            lv_obj_set_style_bg_color(target_obj, lv_color_hex(0xff00ff), LV_INDEV_STATE_RELEASED);
        }
    }
    else
    {
        if (code == LV_EVENT_PRESSED)
        {
            printf("parent %p LV_EVENT_PRESSED\r\n", parent);
            lv_obj_set_style_bg_color(parent, lv_color_hex(0xffff00), LV_STATE_PRESSED);
        }
        else if(code == LV_EVENT_RELEASED)
        {
            printf("parent %p LV_EVENT_RELEASED\r\n", parent);
            lv_obj_set_style_bg_color(parent, lv_color_hex(0xff00ff), LV_INDEV_STATE_RELEASED);
        }
    }
}

void event_bubble(lv_obj_t *screen)
{
    lv_obj_t *obj1 = lv_obj_create(screen);
    lv_obj_set_size(obj1, 500, 400);
    lv_obj_center(obj1);
    lv_obj_add_event_cb(obj1, my_event_cb_bubble, LV_EVENT_ALL, NULL);

    lv_obj_t *obj2 = lv_obj_create(obj1);
    lv_obj_set_size(obj2, 450, 350);
    lv_obj_center(obj2);
    lv_obj_add_flag(obj2, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_event_cb(obj2, my_event_cb_bubble, LV_EVENT_ALL, NULL);

    lv_obj_t *obj3 = lv_obj_create(obj2);
    lv_obj_set_size(obj3, 400, 300);
    lv_obj_center(obj3);
    lv_obj_add_flag(obj3, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_event_cb(obj3, my_event_cb_bubble, LV_EVENT_ALL, NULL);

    lv_obj_t *obj4 = lv_obj_create(obj3);
    lv_obj_set_size(obj4, 350, 250);
    lv_obj_center(obj4);
    lv_obj_add_flag(obj4, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_event_cb(obj4, my_event_cb_bubble, LV_EVENT_ALL, NULL);

    lv_obj_add_event_cb(obj4, my_event_cb_bubble, LV_EVENT_ALL, NULL);
}


// 显示jpg图片
void jpg_display(lv_obj_t *screen)
{  
    LV_IMG_DECLARE(pic1);
    lv_obj_t *img1 = lv_img_create(screen);
    lv_img_set_src(img1, &pic1);
}


// 创建按钮和开关
void btn_and_sw(lv_obj_t *screen)
{
    lv_obj_t *btn = lv_btn_create(screen); // 以屏幕为父对象，创建一个按钮
    lv_obj_t *sw = lv_switch_create(btn); // 以按钮为父对象，创建一个开关
    lv_obj_set_size(sw, 50, 20);
    lv_obj_align(btn, LV_ALIGN_TOP_RIGHT, 0, 0);
}


// 创建电弧270°
void arc270(lv_obj_t *screen)
{
    lv_obj_t *arc = lv_arc_create(screen);
    lv_obj_align(arc, LV_ALIGN_CENTER, 0, -100);
}

// 设置电弧360°
void arc360(lv_obj_t *screen)
{   
    lv_obj_t *spinner = lv_spinner_create(screen, 1000, 60);
    lv_obj_set_size(spinner, 200, 200);
    lv_obj_align(spinner, LV_ALIGN_CENTER, 0, 100);
}


// 显示一个符号图片
void symbol_img(lv_obj_t *screen)
{
    lv_obj_t *img_symbol1 = lv_img_create(screen);
    lv_img_set_src(img_symbol1, LV_SYMBOL_WIFI); // 显示一个符号
    lv_obj_align(img_symbol1, LV_ALIGN_CENTER, 0, 0);
}


// 设置背景颜色
void color_bg(lv_obj_t *screen)
{
    lv_color_t c = lv_color_hex(0xD8BFD8);
    lv_obj_set_style_bg_color(screen, c, LV_STATE_DEFAULT);
}


// 创建滑条样式
void create_style(lv_obj_t *screen)
{
    static lv_style_t style_main;
    lv_style_init(&style_main);
    lv_style_set_bg_color(&style_main, lv_palette_main(LV_PALETTE_ORANGE)); // 设置背景颜色
    lv_style_set_outline_color(&style_main, lv_color_black());              // 设置轮廓颜色
    lv_style_set_outline_width(&style_main, 10);                            // 设置轮廓宽度

    static lv_style_t style_indic;
    lv_style_init(&style_indic);
    lv_style_set_bg_color(&style_indic, lv_palette_lighten(LV_PALETTE_RED, 3)); // 设置背景颜色
    lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_RED));  // 设置渐变颜色
    lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_HOR);                    // 设置渐变方向

    static lv_style_t style_indic_pr;
    lv_style_init(&style_indic_pr);
    lv_style_set_shadow_color(&style_indic_pr, lv_palette_main(LV_PALETTE_RED)); // 设置阴影颜色
    lv_style_set_shadow_width(&style_indic_pr, 10);                              // 设置阴影宽度
    lv_style_set_shadow_spread(&style_indic_pr, 3);                              // 设置阴影扩展

    /*Create an object with the new style_pr*/
    lv_obj_t * obj = lv_slider_create(lv_scr_act());
    lv_obj_add_style(obj, &style_main, LV_PART_MAIN);       //  添加样式到主部分
    lv_obj_add_style(obj, &style_indic, LV_PART_INDICATOR); // 添加样式到指示器部分
    lv_obj_add_style(obj, &style_indic_pr,
                    LV_PART_INDICATOR | LV_STATE_PRESSED); // 添加样式到指示器部分并设置按下状态样式
    lv_slider_set_value(obj, 70, LV_ANIM_OFF);
    lv_obj_center(obj);
}

// 创建字体
void font_test(lv_obj_t *screen)
{
    lv_obj_t *label = lv_label_create(screen);
    lv_obj_center(label);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_32, 0);
    lv_label_set_text(label, "GIVE ME A G\nGIVE ME A E\nGIVE ME A M\nGEM\n");

    // lv_obj_set_style_text_font(label, &lv_font_simsun_16_cjk, 0);
    // lv_label_set_text(label, "您好");
}


// gif显示
void gif_show(lv_obj_t *screen)
{
    // gif显示
    LV_IMG_DECLARE(kakaloto);
    lv_obj_t *gif1 = lv_gif_create(screen);
    lv_gif_set_src(gif1, &kakaloto);
    lv_obj_align(gif1, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(gif1, 200, 200);
}

// freetype文字显示
void freetype_test(lv_obj_t *screen)
{
    static lv_ft_info_t info;

    info.name = "/root/work/SourceHanSansCN-Normal-2.otf";
    info.weight = 32;
    info.style = FT_FONT_STYLE_NORMAL;
    info.mem = NULL;
    if (!lv_ft_font_init(&info))
    {
        LV_LOG_ERROR("create fail");
    }

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_palette_main(LV_PALETTE_PINK));
    lv_style_set_bg_opa(&style, LV_OPA_COVER);
    lv_style_set_radius(&style, 10);
    lv_style_set_pad_all(&style, 20);

    lv_style_set_text_font(&style, info.font);
    lv_style_set_text_color(&style, lv_color_black());
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);

    lv_obj_t *label = lv_label_create(screen);
    lv_obj_add_style(label, &style, 0);
    lv_label_set_text(label, "GIVE ME A \nG\nGIVE ME A \nE\nGIVE ME A \nM\nGIVE ME A \nG!E!M!\n");
    // lv_label_set_text(label, "安惠嵘是个大坏蛋！！！\n");
    
    lv_obj_set_size(label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

    lv_obj_center(label);
}


// 键盘控件
#if LV_USE_KEYBOARD

static void ta_event_cb(lv_event_t *e)
{
    // 获取事件
    lv_event_code_t code = lv_event_get_code(e);
    // 获取触发事件的对象（文本区域）
    lv_obj_t *ta = lv_event_get_target(e);
    // 获取用户数据（键盘对象）
    lv_obj_t *kb = lv_event_get_user_data(e);

    // 如果事件是获得焦点
    if (code == LV_EVENT_FOCUSED)
    {
        // 将键盘关联到当前文本区域
        lv_keyboard_set_textarea(kb, ta);
        // 显示键盘（移除隐藏标志）
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }

    // 如果事件是失去焦点
    if (code == LV_EVENT_DEFOCUSED)
    {
        // 取消键盘与文本区域的关联
        lv_keyboard_set_textarea(kb, NULL);
        // 隐藏键盘（添加隐藏标志）
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
}

void lv_example_keyboard(lv_obj_t *screen)
{
    /* 创建一个键盘对象，用于文本区域配合使用 */ 
    lv_obj_t *kb = lv_keyboard_create(screen);

    // 创建第一个文本区域，键盘将在此输入
    lv_obj_t *ta1;
    ta1 = lv_textarea_create(screen);
    // 设置文本区域位置：左上角，偏移(10, 10)
    lv_obj_align(ta1, LV_ALIGN_TOP_LEFT, 10, 10);
    // 为文本区域添加事件回调，所有事件都会触发ta_event_cb
    lv_obj_add_event_cb(ta1, ta_event_cb, LV_EVENT_ALL, kb);
    // 设置占位符文本
    lv_textarea_set_placeholder_text(ta1, "Hello");
    // 设置文本区域大小
    lv_obj_set_size(ta1, 140, 80);

    /* 创建第二个文本区域 */
    lv_obj_t *ta2;
    ta2 = lv_textarea_create(screen);
    // 设置文本区域位置：右上角，偏移(-10, 10)
    lv_obj_align(ta2, LV_ALIGN_TOP_RIGHT, -10, 10);
    // 为文本区域添加事件回调
    lv_obj_add_event_cb(ta2, ta_event_cb, LV_EVENT_ALL, kb);
    // 设置文本区域大小
    lv_obj_set_size(ta2, 140, 80);

    // 默认将键盘关联到第一个文本区域
    lv_keyboard_set_textarea(kb, ta1);
}
#endif


// 多图片切换
uint8_t num = 0;
char *jpg_path[] = 
{
    "A:/root/1.jpg",
    "A:/root/2.jpg",
    "A:/root/3.jpg"
};

void btn_event_cb(lv_event_t *e)
{
    lv_obj_t *img = lv_event_get_user_data(e);
    num++;
    num = num > 2 ? 0 : num;
    lv_img_set_src(img, jpg_path[num]);
}

void multijpg_test(lv_obj_t *screen)
{
    lv_obj_t *btn = lv_btn_create(screen);
    lv_obj_set_size(btn, 100, 100);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_move_foreground(btn);

    lv_obj_t *img = lv_img_create(screen);
    lv_img_set_src(img, "A:/root/1.jpg");
    lv_obj_center(img);
    lv_obj_move_background(img);

    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_PRESSED, img);
}


// 优化图片切换
char *png_path[] = 
{
    "A:/root/1.png",
    "A:/root/2.png",
    "A:/root/3.png"
};

void img_decoder_timer_cb(struct _lv_timer_t *timer)
{
    uint8_t next_index = *(uint8_t *)(timer->user_data);
    lv_img_decoder_dsc_t dsc;
    lv_img_decoder_open(&dsc, png_path[next_index], lv_color_white(), 0);
    lv_img_decoder_close(&dsc);

    lv_timer_del(timer);
}

void png_event_cb(lv_event_t *e)
{
    lv_obj_t *img = lv_event_get_user_data(e);
    lv_obj_t *btn = lv_event_get_target(e);
    static uint8_t current_index = 0;

    // 1. 从缓存显示当前图片
    lv_img_set_src(img, png_path[current_index]);
    lv_obj_move_foreground(btn);    // 将按钮移到最上层

    // 2. 移除上张缓存，给显示下一张缓存留出空间
    signed char prev_index = current_index - 1;
    prev_index = prev_index < 0 ? (sizeof(png_path) / sizeof(png_path[0]) - 1) : prev_index;
    lv_img_cache_invalidate_src(png_path[prev_index]);

    static uint8_t next_index = 0;
    next_index = (current_index + 1) % (sizeof(png_path) / sizeof(png_path[0]));

    // 3. 开启定时器缓存下一张
    lv_timer_t *timer = lv_timer_create(img_decoder_timer_cb, 500, &next_index);
    lv_timer_set_repeat_count(timer, 1);    // 让定时器只执行一次

    // 更新索引
    current_index = next_index;
}

void png_improvement(lv_obj_t *screen)
{
    lv_obj_t *btn = lv_btn_create(screen);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_move_foreground(btn);    // 将按钮移到最上层

    lv_obj_t *img = lv_img_create(screen);
    lv_obj_center(img);

    lv_obj_add_event_cb(btn, png_event_cb, LV_EVENT_PRESSED, img);
}


// 拼音键盘输入示例
static void pinyin_ta_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta = lv_event_get_target(e);
    lv_obj_t *ime = lv_event_get_user_data(e);
    lv_ime_pinyin_t *pinyin_ime = (lv_ime_pinyin_t *)ime;

    // 如果事件是获得焦点
    if (code == LV_EVENT_FOCUSED)
    {
        lv_keyboard_set_textarea(pinyin_ime->kb, ta);
        lv_ime_pinyin_show((lv_obj_t *)pinyin_ime);
    }
    // 如果事件是失去焦点
    else if (code == LV_EVENT_DEFOCUSED)
    {
        lv_obj_clear_state(ta, LV_STATE_FOCUSED);
        lv_keyboard_set_textarea(pinyin_ime->kb, NULL);
        lv_ime_pinyin_hide((lv_obj_t *)pinyin_ime);     // 隐藏输入法
        lv_indev_reset(NULL, ta);                       // 清除输入法焦点
    }
    // 输入法输入完成
    else if (code == LV_EVENT_READY)
    {
        lv_obj_clear_state(ta, LV_STATE_FOCUSED);
        lv_keyboard_set_textarea(pinyin_ime->kb, NULL);
        lv_ime_pinyin_hide((lv_obj_t *)pinyin_ime);
        lv_indev_reset(NULL, ta);
    }
}

void lv_pinyin_example(lv_obj_t *screen)
{
    lv_obj_t *pinyin_ime = lv_ime_pinyin_create(screen);
    lv_obj_set_style_text_font(pinyin_ime, &lv_font_simsun_16_cjk, 0);

    extern lv_pinyin_dict_t lv_ime_pinyin_def_dict[];
    lv_ime_pinyin_set_dict(pinyin_ime, lv_ime_pinyin_def_dict);

    /* ta1 */
    lv_obj_t *ta1 = lv_textarea_create(screen);
    lv_textarea_set_one_line(ta1, true);
    lv_obj_set_style_text_font(ta1, &lv_font_simsun_16_cjk, 0);
    lv_obj_align(ta1, LV_ALIGN_TOP_LEFT, 0, 0);

    /*Create a keyboard and add it to ime_pinyin*/
    lv_obj_t *kb = lv_keyboard_create(screen);
    lv_ime_pinyin_set_keyboard(pinyin_ime, kb);
    lv_keyboard_set_textarea(kb, ta1);

    // 设置候选面板大小和位置
    lv_obj_t *cand_panel = lv_ime_pinyin_get_cand_panel(pinyin_ime);
    lv_obj_set_size(cand_panel, LV_PCT(100), LV_PCT(10));
    lv_obj_align_to(cand_panel, kb, LV_ALIGN_OUT_TOP_MID, 0, 0);

    lv_obj_add_event_cb(ta1, pinyin_ta_event_cb, LV_EVENT_ALL, pinyin_ime);
}


// 屏幕切换
lv_obj_t *screen1;
lv_obj_t *screen2;

void lv_event_cb_t1(lv_event_t *e)
{
    lv_scr_load_anim(screen2, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, false);
}

void lv_event_cb_t2(lv_event_t *e)
{
    lv_scr_load_anim(screen1, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, false);
}

void create_screen1(void)
{
    screen1 = lv_obj_create(NULL);
    // 设置屏幕的背景为不透明，防止残留
    lv_obj_set_style_bg_opa(screen1, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(screen1, lv_color_hex(0x000000), LV_PART_MAIN);

    // 添加标题
    lv_obj_t *title = lv_label_create(screen1);
    lv_label_set_text(title, "screen 1");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);
    lv_obj_set_size(title, 150, 50);
    lv_obj_set_style_bg_opa(title, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(title, lv_color_hex(0xffffff), LV_PART_MAIN);

    // 添加切换到屏幕2的按钮
    lv_obj_t *btn1 = lv_btn_create(screen1);
    lv_obj_set_size(btn1,200, 50);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -50);
    lv_obj_add_event_cb(btn1, lv_event_cb_t1, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btn1_label = lv_label_create(btn1);
    lv_label_set_text(btn1_label, "to screen2");
    lv_obj_center(btn1_label);
}

void create_screen2(void)
{
    screen2 = lv_obj_create(NULL);
    // 设置屏幕的背景为不透明，防止残留
    lv_obj_set_style_bg_opa(screen2, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(screen2, lv_color_hex(0x000000), LV_PART_MAIN);

    // 添加标题
    lv_obj_t *title = lv_label_create(screen2);
    lv_label_set_text(title, "screen 2");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);
    lv_obj_set_size(title, 150, 50);
    lv_obj_set_style_bg_opa(title, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(title, lv_color_hex(0xffffff), LV_PART_MAIN);

    // 添加返回按钮
    lv_obj_t *btn = lv_btn_create(screen2);
    lv_obj_set_size(btn, 200, 50);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(btn, lv_event_cb_t2, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "to screen1");
    lv_obj_center(btn_label);

    // 添加一些控件
    lv_obj_t *slider = lv_slider_create(screen2);
    lv_obj_set_width(slider, 200);
    lv_obj_align(slider, LV_ALIGN_CENTER, 0, 80);
    lv_slider_set_value(slider, 50, LV_ANIM_OFF);
}


void lv_usr_test(void)
{
    lv_obj_t *screen = lv_scr_act(); // 获取当前活动屏幕
    
    
    create_screen1();
    create_screen2();
    lv_scr_load(screen1);
    

    // color_bg(screen);

    // lv_pinyin_example(screen);

    // multijpg_test(screen);

    // jpg_display(screen);

    // lv_example_keyboard(screen);

    // freetype_test(screen);

    // gif_show(screen);

    // create_style(screen);

    // font_test(screen);

    // event_bubble(screen);
    // even_callback(screen);

    // create_login_register_ui(screen);
}