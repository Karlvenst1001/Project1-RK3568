#include "../src/music.h"

// 音乐播放控制变量
pid_t mplayer_music_pid = -1; // mplayer进程ID
int is_playing = 0;           // 播放状态标志
int current_volume = 50;      // 当前音量(0-100)
char *current_song = NULL;    // 当前播放歌曲
char **music_list = NULL;     // 音乐文件列表
int music_count = 0;          // 音乐文件数量
int current_index = -1;       // 当前播放索引

// 初始化音乐播放器
void init_music_player()
{
    // 创建FIFO管道用于控制mplayer
    if (access(MPLAYER_FIFO, F_OK) == -1)
    {
        system("mkfifo /tmp/mplayer_fifo");
    }

    // 扫描音乐目录
    DIR *dir;
    struct dirent *ent;
    const char *music_dir = "/root/musics";

    // 第一次扫描获取文件数量
    if ((dir = opendir(music_dir)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_type == DT_REG && strstr(ent->d_name, ".mp3"))
            {
                music_count++;
            }
        }
        closedir(dir);
    }

    // 分配内存并存储文件名
    if (music_count > 0)
    {
        music_list = malloc(music_count * sizeof(char *));
        int index = 0;
        dir = opendir(music_dir);

        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_type == DT_REG && strstr(ent->d_name, ".mp3"))
            {
                music_list[index] = strdup(ent->d_name);
                index++;
            }
        }
        closedir(dir);
    }

    // 更新Dropdown内容
    if (music_count > 0)
    {
        char options[2048] = "";
        for (int i = 0; i < music_count; i++)
        {
            strcat(options, music_list[i]);
            if (i < music_count - 1)
                strcat(options, "\n");
        }
        lv_dropdown_set_options(ui_Dropdownmusic, options);
    }
}

// 启动mplayer播放指定音乐
void play_music(const char *filename)
{
    // 如果已经有mplayer在运行，先停止
    if (mplayer_music_pid > 0)
    {
        kill(mplayer_music_pid, SIGTERM);
        waitpid(mplayer_music_pid, NULL, 0);
        mplayer_music_pid = -1;
    }

    // 构建完整路径
    char full_path[256];
    snprintf(full_path, sizeof(full_path), "/root/musics/%s", filename);
    // 启动mplayer
    pid_t pid = fork();
    if (pid == 0)
    { // 子进程
        // 重定向输出到/dev/null
        int dev_null = open("/dev/null", O_WRONLY);
        dup2(dev_null, STDOUT_FILENO);
        dup2(dev_null, STDERR_FILENO);
        close(dev_null);

        // 执行mplayer命令
        execlp("mplayer", "mplayer", "-slave", "-quiet",
               "-input", "file=/tmp/mplayer_fifo",
               "-ao", "alsa:device=hw=0,0", // 使用3.5mm耳机接口
               "-volume", "40",             // 初始音量
               full_path, NULL);

        // 如果exec失败
        perror("execlp failed");

        exit(1);
    }
    else if (pid > 0)
    { // 父进程
        mplayer_music_pid = pid;
        current_song = strdup(filename);
        is_playing = 1;

        // 更新UI状态
        _ui_flag_modify(ui_playmusicbtn, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        _ui_flag_modify(ui_playmusic, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        _ui_flag_modify(ui_stopmusicbtn, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_flag_modify(ui_stopmusic, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);

        // 查找当前音乐索引
        for (int i = 0; i < music_count; i++)
        {
            if (strcmp(music_list[i], filename) == 0)
            {
                current_index = i;
                break;
            }
        }
    }
}

// 向mplayer发送命令
void send_mplayer_music_command(const char *cmd)
{
    FILE *fifo = fopen(MPLAYER_FIFO, "w");
    if (fifo)
    {
        fprintf(fifo, "%s\n", cmd);
        fclose(fifo);
    }
}

// 暂停/继续音乐
void toggle_pause()
{
    if (mplayer_music_pid <= 0)
        return; // 没有播放中的音乐，直接返回

    send_mplayer_music_command("pause");
    is_playing = !is_playing; // 切换播放状态

    if (is_playing)
    {
        // 正在播放：隐藏播放按钮，显示暂停按钮
        _ui_flag_modify(ui_playmusicbtn, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        _ui_flag_modify(ui_playmusic, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        _ui_flag_modify(ui_stopmusicbtn, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_flag_modify(ui_stopmusic, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
    }
    else
    {
        // 已暂停：显示播放按钮，隐藏暂停按钮
        _ui_flag_modify(ui_playmusicbtn, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_flag_modify(ui_playmusic, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_flag_modify(ui_stopmusicbtn, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        _ui_flag_modify(ui_stopmusic, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
    }
}


// 停止音乐
void stop_music()
{
    if (mplayer_music_pid > 0)
    {
        send_mplayer_music_command("quit");
        waitpid(mplayer_music_pid, NULL, 0);
        mplayer_music_pid = -1;
    }
    is_playing = 0;
    current_song = NULL;
    current_index = -1;
    free(current_song);

    // 更新UI
    _ui_flag_modify(ui_playmusicbtn, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
    _ui_flag_modify(ui_playmusic, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
    _ui_flag_modify(ui_stopmusicbtn, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
    _ui_flag_modify(ui_stopmusic, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
}

// 设置音量
void set_volume(int vol)
{
    if (vol < 0)
        vol = 0;
    if (vol > 100)
        vol = 100;

    char cmd[32];
    snprintf(cmd, sizeof(cmd), "volume %d 1", vol);
    send_mplayer_music_command(cmd);
    current_volume = vol;
}

// 播放下一首
void play_next()
{
    if (music_count == 0)
        return;

    current_index = (current_index + 1) % music_count;
    play_music(music_list[current_index]);
}

// 播放上一首
void play_prev()
{
    if (music_count == 0)
        return;

    current_index = (current_index - 1 + music_count) % music_count;
    play_music(music_list[current_index]);
}

// 事件处理函数
void ui_event_Dropdownmusic(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_VALUE_CHANGED)
    {
        char selected[128];
        lv_dropdown_get_selected_str(ui_Dropdownmusic, selected, sizeof(selected));

        // 如果当前正在播放，先停止
        if (mplayer_music_pid > 0)
        {
            stop_music();
        }

        // 播放选中的音乐
        play_music(selected);
    }
}


// 添加播放按钮点击事件处理函数
void ui_event_playmusicbtn(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_PRESSED)
    {
        if (is_playing == 0)
        {
            // 如果是暂停状态，继续播放
            toggle_pause();
        }
        else if (current_song != NULL)
        {
            // 如果没有播放，从头播放当前歌曲
            play_music(current_song);
        }
    }
}

// 添加暂停按钮点击事件处理函数
void ui_event_stopmusicbtn(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_PRESSED)
    {
        // 点击暂停按钮时切换到暂停状态
        toggle_pause();
    }
}


void ui_event_backmusicbtn(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED)
    {
        play_prev();
    }
}

void ui_event_forwardmusicbtn(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_PRESSED)
    {
        play_next();
    }
}

void ui_event_Slider6_value_changed(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_VALUE_CHANGED)
    {
        int volume = lv_slider_get_value(ui_Slider6);
        set_volume(volume);
    }

    if (event_code == LV_EVENT_DEFOCUSED)
    {
        _ui_flag_modify(ui_Slider6, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
    }
}