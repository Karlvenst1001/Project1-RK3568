#ifndef __SNACK_H__
#define __SNACK_H__

#include "../ui.h"

// 函数前置声明
void init_game_board(void);
void init_snake_game(void);
void generate_food(void);
void update_board_display(void);
void update_game(lv_timer_t *timer);
void start_game_handler(lv_event_t *e);
void pause_game_handler(lv_event_t *e);
void create_control_buttons(void);
void init_snake_app(void);
void cleanup_snake_app(void);

/* ================== 游戏 =================*/
// 在ui.h中添加声明
// 贪吃蛇游戏相关数据结构
#define GRID_SIZE 20
#define CELL_SIZE 20
#define BOARD_WIDTH 20
#define BOARD_HEIGHT 20
#define BOARD_OFFSET_X 150
#define BOARD_OFFSET_Y 100

// 确保边框不影响布局
#define BOARD_BORDER_WIDTH 0
#define CELL_BORDER_WIDTH 0

typedef enum {
    EMPTY,
    SNAKE_BODY,
    SNAKE_HEAD,
    FOOD
} CellType;

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct {
    int x;
    int y;
} Position;

#endif