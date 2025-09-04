#include "../src/snack.h"

// 游戏状态
static CellType board[BOARD_WIDTH][BOARD_HEIGHT];
static Position snake[BOARD_WIDTH * BOARD_HEIGHT];
static int snake_length = 3;
static Direction current_direction = RIGHT;
static Position food_position;
static int game_active = 0;
static int game_over = 0;
static int score = 0;
static int high_score = 0;

// UI元素
static lv_obj_t *game_board_container;
static lv_obj_t *grid_cells[BOARD_WIDTH][BOARD_HEIGHT];
static lv_obj_t *score_label;
static lv_obj_t *high_score_label;
static lv_obj_t *game_status_label;
static lv_timer_t *game_timer;

// 控制按钮
static lv_obj_t *up_btn;
static lv_obj_t *down_btn;
static lv_obj_t *left_btn;
static lv_obj_t *right_btn;


// 控制按钮事件处理
void control_btn_handler(lv_event_t *e)
{
    if (!game_active || game_over)
    {
        return;
    }

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (code == LV_EVENT_PRESSED)
    {
        if (target == up_btn)
        {
            if (current_direction != DOWN)
                current_direction = UP;
        }
        else if (target == down_btn)
        {
            if (current_direction != UP)
                current_direction = DOWN;
        }
        else if (target == left_btn)
        {
            if (current_direction != RIGHT)
                current_direction = LEFT;
        }
        else if (target == right_btn)
        {
            if (current_direction != LEFT)
                current_direction = RIGHT;
        }
    }
}

// 开始游戏按钮事件处理
void start_game_handler(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_CLICKED)
    {
        if (!game_timer)
        {
            // 创建游戏定时器
            game_timer = lv_timer_create(update_game, 150, NULL); // 150ms更新一次
        }

        if (game_over)
        {
            init_snake_game();
        }
        else if (!game_active)
        {
            init_snake_game();
        }

        // 更新按钮文本
        lv_label_set_text(ui_StartLabel, game_over ? "restart" : "gaming");
    }
}

// 暂停游戏按钮事件处理
void pause_game_handler(lv_event_t *e)
{
    if (!game_timer)
        return;

    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_CLICKED)
    {
        if (game_active)
        {
            game_active = 0;
            lv_label_set_text(game_status_label, "game pause");
            lv_obj_set_style_text_color(game_status_label, lv_color_hex(0x707070), 0);
        }
        else if (!game_over)
        {
            game_active = 1;
            lv_label_set_text(game_status_label, "gaming...");
            lv_obj_set_style_text_color(game_status_label, lv_color_hex(0x707070), 0);
        }
    }
}

// 创建控制按钮
void create_control_buttons()
{
    // 创建控制按钮容器
    lv_obj_t *control_container = lv_obj_create(ui_Screen5);
    lv_obj_set_size(control_container, 250, 250);
    lv_obj_set_pos(control_container, BOARD_OFFSET_X + BOARD_WIDTH * CELL_SIZE + 50, BOARD_OFFSET_Y + 100);
    lv_obj_set_style_bg_opa(control_container, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_opa(control_container, LV_OPA_TRANSP, LV_PART_MAIN);

    // 创建上按钮
    up_btn = lv_btn_create(control_container);
    lv_obj_set_size(up_btn, 80, 50);
    lv_obj_set_pos(up_btn, 60, 10);
    lv_obj_set_style_bg_color(up_btn, lv_color_hex(0x707070), LV_PART_MAIN);
    lv_obj_t *up_label = lv_label_create(up_btn);
    lv_label_set_text(up_label, "w");
    lv_obj_set_style_text_font(up_label, &lv_font_montserrat_20, 0);
    lv_obj_center(up_label);
    lv_obj_add_event_cb(up_btn, control_btn_handler, LV_EVENT_ALL, NULL);

    // 创建下按钮
    down_btn = lv_btn_create(control_container);
    lv_obj_set_size(down_btn, 80, 50);
    lv_obj_set_pos(down_btn, 60, 150);
    lv_obj_set_style_bg_color(down_btn, lv_color_hex(0x707070), LV_PART_MAIN);
    lv_obj_t *down_label = lv_label_create(down_btn);
    lv_label_set_text(down_label, "s");
    lv_obj_set_style_text_font(down_label, &lv_font_montserrat_20, 0);
    lv_obj_center(down_label);
    lv_obj_add_event_cb(down_btn, control_btn_handler, LV_EVENT_ALL, NULL);

    // 创建左按钮
    left_btn = lv_btn_create(control_container);
    lv_obj_set_size(left_btn, 50, 80);
    lv_obj_set_pos(left_btn, 0, 75);
    lv_obj_set_style_bg_color(left_btn, lv_color_hex(0x707070), LV_PART_MAIN);
    lv_obj_t *left_label = lv_label_create(left_btn);
    lv_label_set_text(left_label, "a");
    lv_obj_set_style_text_font(left_label, &lv_font_montserrat_20, 0);
    lv_obj_center(left_label);
    lv_obj_add_event_cb(left_btn, control_btn_handler, LV_EVENT_ALL, NULL);

    // 创建右按钮
    right_btn = lv_btn_create(control_container);
    lv_obj_set_size(right_btn, 50, 80);
    lv_obj_set_pos(right_btn, 150, 75);
    lv_obj_set_style_bg_color(right_btn, lv_color_hex(0x707070), LV_PART_MAIN);
    lv_obj_t *right_label = lv_label_create(right_btn);
    lv_label_set_text(right_label, "d");
    lv_obj_set_style_text_font(right_label, &lv_font_montserrat_20, 0);
    lv_obj_center(right_label);
    lv_obj_add_event_cb(right_btn, control_btn_handler, LV_EVENT_ALL, NULL);
}

// 初始化贪吃蛇游戏
void init_snake_app()
{
    // 初始化随机数种子
    srand(time(NULL));

    // 创建游戏板
    init_game_board();

    // 创建控制按钮
    create_control_buttons();

    // 更新开始按钮文本
    lv_label_set_text(ui_StartLabel, "Start");

    // 添加开始游戏按钮事件处理
    lv_obj_add_event_cb(ui_Startgame, start_game_handler, LV_EVENT_CLICKED, NULL);

    // 添加声音按钮作为暂停按钮
    lv_obj_add_event_cb(ui_Pausegame, pause_game_handler, LV_EVENT_CLICKED, NULL);

    // 初始状态
    lv_label_set_text_fmt(high_score_label, "Best: %d", high_score);
}

// 清理游戏资源
void cleanup_snake_app()
{
    if (game_timer)
    {
        lv_timer_del(game_timer);
        game_timer = NULL;
    }

    if (game_board_container)
    {
        lv_obj_del(game_board_container);
        game_board_container = NULL;
    }

    if (score_label)
    {
        lv_obj_del(score_label);
        score_label = NULL;
    }

    if (high_score_label)
    {
        lv_obj_del(high_score_label);
        high_score_label = NULL;
    }

    if (game_status_label)
    {
        lv_obj_del(game_status_label);
        game_status_label = NULL;
    }
}



// 初始化游戏板
void init_game_board()
{
    // 创建游戏板容器
    game_board_container = lv_obj_create(ui_Screen5);

    // 精确计算容器大小（考虑边框）
    int container_width = (BOARD_WIDTH * CELL_SIZE) + (2 * BOARD_BORDER_WIDTH);
    int container_height = (BOARD_HEIGHT * CELL_SIZE) + (2 * BOARD_BORDER_WIDTH);

    lv_obj_set_size(game_board_container, container_width, container_height);
    lv_obj_set_pos(game_board_container, BOARD_OFFSET_X, BOARD_OFFSET_Y);

    // 禁用游戏板容器的滚动
    lv_obj_clear_flag(game_board_container, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_bg_color(game_board_container, lv_color_hex(0x2C3E50), LV_PART_MAIN);
    lv_obj_set_style_border_width(game_board_container, BOARD_BORDER_WIDTH, LV_PART_MAIN);
    lv_obj_set_style_border_color(game_board_container, lv_color_hex(0x34495E), LV_PART_MAIN);
    lv_obj_set_style_pad_all(game_board_container, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(game_board_container, 0, LV_PART_MAIN);

    // 创建网格单元格
    for (int y = 0; y < BOARD_HEIGHT; y++)
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            lv_obj_t *cell = lv_obj_create(game_board_container);
            lv_obj_set_size(cell, CELL_SIZE, CELL_SIZE);

            // 精确计算位置（考虑边框）
            int pos_x = (x * CELL_SIZE) + BOARD_BORDER_WIDTH;
            int pos_y = (y * CELL_SIZE) + BOARD_BORDER_WIDTH;
            lv_obj_set_pos(cell, pos_x, pos_y);

            // 禁用滚动并设置样式
            lv_obj_clear_flag(cell, LV_OBJ_FLAG_SCROLLABLE);

            lv_obj_set_style_bg_color(cell, lv_color_hex(0x34495E), LV_PART_MAIN);
            lv_obj_set_style_border_width(cell, CELL_BORDER_WIDTH, LV_PART_MAIN);
            lv_obj_set_style_border_color(cell, lv_color_hex(0x2C3E50), LV_PART_MAIN);
            lv_obj_set_style_radius(cell, 0, LV_PART_MAIN);
            lv_obj_set_style_pad_all(cell, 0, LV_PART_MAIN);

            grid_cells[x][y] = cell;
        }
    }

    // 创建分数显示
    score_label = lv_label_create(ui_Screen5);
    lv_label_set_text(score_label, "Score: 0");
    lv_obj_set_pos(score_label, BOARD_OFFSET_X, BOARD_OFFSET_Y - 30);
    lv_obj_set_style_text_font(score_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(score_label, lv_color_hex(0x707070), 0);

    high_score_label = lv_label_create(ui_Screen5);
    lv_label_set_text(high_score_label, "Best: 0");
    lv_obj_set_pos(high_score_label, BOARD_OFFSET_X + 200, BOARD_OFFSET_Y - 30);
    lv_obj_set_style_text_font(high_score_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(high_score_label, lv_color_hex(0xD81E06), 0);

    game_status_label = lv_label_create(ui_Screen5);
    lv_label_set_text(game_status_label, "press start to game");
    lv_obj_set_pos(game_status_label, BOARD_OFFSET_X, BOARD_OFFSET_Y + BOARD_HEIGHT * CELL_SIZE + 10);
    lv_obj_set_style_text_font(game_status_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(game_status_label, lv_color_hex(0x707070), 0);
}

// 初始化游戏状态
void init_snake_game()
{
    // 重置游戏板
    for (int y = 0; y < BOARD_HEIGHT; y++)
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            board[x][y] = EMPTY;
            lv_obj_set_style_bg_color(grid_cells[x][y], lv_color_hex(0x34495E), LV_PART_MAIN);
        }
    }

    // 初始化蛇
    int start_x = BOARD_WIDTH / 4;
    int start_y = BOARD_HEIGHT / 2;
    snake_length = 3;

    for (int i = 0; i < snake_length; i++)
    {
        snake[i].x = start_x + i;
        snake[i].y = start_y;
        board[snake[i].x][snake[i].y] = (i == snake_length - 1) ? SNAKE_HEAD : SNAKE_BODY;
    }

    // 放置食物
    generate_food();

    // 重置游戏状态
    current_direction = RIGHT;
    game_active = 1;
    game_over = 0;
    score = 0;
    lv_label_set_text_fmt(score_label, "Score: %d", score);
    lv_obj_set_style_text_color(score_label, lv_color_hex(0x707070), 0);

    // 更新游戏状态显示
    lv_label_set_text(game_status_label, "gaming...");
    lv_obj_set_style_text_color(game_status_label, lv_color_hex(0x707070), 0);

    // 更新UI
    update_board_display();
}

// 生成食物
void generate_food()
{
    int x, y;
    int attempts = 0;
    const int max_attempts = BOARD_WIDTH * BOARD_HEIGHT;

    do
    {
        x = rand() % BOARD_WIDTH;
        y = rand() % BOARD_HEIGHT;

        // 防止无限循环
        if (attempts > max_attempts)
        {
            // 找不到合适位置，随机选择一个
            break;
        }

    } while (board[x][y] != EMPTY);

    food_position.x = x;
    food_position.y = y;
    board[x][y] = FOOD;
}

// 更新游戏板显示
void update_board_display()
{
    for (int y = 0; y < BOARD_HEIGHT; y++)
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT)
            {
                switch (board[x][y])
                {
                case EMPTY:
                    lv_obj_set_style_bg_color(grid_cells[x][y], lv_color_hex(0x34495E), LV_PART_MAIN);
                    break;
                case SNAKE_BODY:
                    lv_obj_set_style_bg_color(grid_cells[x][y], lv_color_hex(0x2ECC71), LV_PART_MAIN);
                    break;
                case SNAKE_HEAD:
                    lv_obj_set_style_bg_color(grid_cells[x][y], lv_color_hex(0x3498DB), LV_PART_MAIN);
                    break;
                case FOOD:
                    lv_obj_set_style_bg_color(grid_cells[x][y], lv_color_hex(0xE74C3C), LV_PART_MAIN);
                    break;
                }
            }
        }
    }
}

// 游戏逻辑更新
void update_game(lv_timer_t *timer)
{
    if (!game_active || game_over)
    {
        return;
    }

    // 计算新的蛇头位置
    Position new_head = snake[snake_length - 1];

    switch (current_direction)
    {
    case UP:
        new_head.y--;
        break;
    case DOWN:
        new_head.y++;
        break;
    case LEFT:
        new_head.x--;
        break;
    case RIGHT:
        new_head.x++;
        break;
    }

    // 修复：确保位置有效后再访问数组
    int position_valid = (new_head.x >= 0 && new_head.x < BOARD_WIDTH &&
                          new_head.y >= 0 && new_head.y < BOARD_HEIGHT);

    // 检查碰撞
    if (!position_valid)
    {
        // 游戏结束
        game_over = 1;
        game_active = 0;

        // 更新最高分
        if (score > high_score)
        {
            high_score = score;
            lv_label_set_text_fmt(high_score_label, "Best: %d", high_score);
        }

        lv_label_set_text(game_status_label, "gameover! press restart");
        lv_obj_set_style_text_color(game_status_label, lv_color_hex(0x707070), 0);

        return;
    }

    // 检查是否撞到自己的身体
    if (board[new_head.x][new_head.y] == SNAKE_BODY) {
        // 游戏结束（撞自己）
        game_over = 1;
        game_active = 0;
        
        if (score > high_score) {
            high_score = score;
            lv_label_set_text_fmt(high_score_label, "Best: %d", high_score);
        }
        
        lv_label_set_text(game_status_label, "gameover! press restart");
        lv_obj_set_style_text_color(game_status_label, lv_color_hex(0x707070), 0);
        
        // 蛇头变红色表示碰撞
        lv_obj_set_style_bg_color(grid_cells[new_head.x][new_head.y], lv_color_hex(0xFF0000), LV_PART_MAIN);
        return;
    }


    // 检查是否吃到食物
    int ate_food = (board[new_head.x][new_head.y] == FOOD);

    // 移动蛇
    if (!ate_food)
    {
        // 移除蛇尾
        Position tail = snake[0];
        // 确保位置有效
        if (tail.x >= 0 && tail.x < BOARD_WIDTH && 
            tail.y >= 0 && tail.y < BOARD_HEIGHT) {
            board[tail.x][tail.y] = EMPTY;
        }

        // 移动蛇身
        for (int i = 0; i < snake_length - 1; i++)
        {
            snake[i] = snake[i + 1];
        }
    }
    else
    {
        // 吃到食物，蛇身增长
        snake_length++;
        score += 10;
        lv_label_set_text_fmt(score_label, "Score: %d", score);
        generate_food();
    }

    // 添加新的蛇头
    // 确保新头部位置有效
    if (new_head.x >= 0 && new_head.x < BOARD_WIDTH && 
        new_head.y >= 0 && new_head.y < BOARD_HEIGHT) {
        // 添加新的蛇头
        board[new_head.x][new_head.y] = SNAKE_HEAD;
    }
    snake[snake_length - 1] = new_head;

    // 更新蛇身位置
    for (int i = 0; i < snake_length - 1; i++)
    {
        // 确保位置有效
        if (snake[i].x >= 0 && snake[i].x < BOARD_WIDTH && 
            snake[i].y >= 0 && snake[i].y < BOARD_HEIGHT) {
            board[snake[i].x][snake[i].y] = SNAKE_BODY;
        }
    }

    // 更新显示
    update_board_display();
}