#include "../src/video.h"

pid_t mplayer_pid = -1;
lv_timer_t *progress_timer = NULL;
char current_video_path[256] = {0};
const char *VIDEO_DIR = "/root/videos";

/* ================== 视频 =================*/
// 发送命令到mplayer
void send_mplayer_command(const char *cmd) {
    if (mplayer_pid > 0) {
        int fifo_fd = open("/tmp/mplayer_fifo", O_WRONLY | O_NONBLOCK);
        if (fifo_fd != -1) {
            write(fifo_fd, cmd, strlen(cmd));
            write(fifo_fd, "\n", 1);
            close(fifo_fd);
        }
    }
}

// 播放视频
void video_play(const char *path) {
    // 停止当前播放
    if (mplayer_pid > 0) {
        video_stop();
    }
    
    // 保存当前视频路径
    strncpy(current_video_path, path, sizeof(current_video_path)-1);
    
    // 创建FIFO用于控制
    mkfifo("/tmp/mplayer_fifo", 0666);

    // 创建mplayer进程
    pid_t pid = fork();
    if (pid == 0) {
        // 子进程：执行mplayer
        execlp("/bin/mplayer", "mplayer", "-slave", "-quiet", 
            "-vo", "fbdev2:/dev/fb0",  // 使用FrameBuffer输出
            "-input", "file=/tmp/mplayer_fifo",
            path, 
            NULL);
        perror("execlp failed");
        exit(1);
    } else if (pid > 0) {
        mplayer_pid = pid;
        
        // 启动进度更新定时器
        if (progress_timer == NULL) {
            progress_timer = lv_timer_create(video_update_progress_timer, 500, NULL);
        }
        
        // 更新UI状态
        _ui_flag_modify(ui_play, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        _ui_flag_modify(ui_playbtn, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        _ui_flag_modify(ui_stop, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_flag_modify(ui_stopbtn, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
    }
}

// 暂停/恢复播放
void video_toggle_pause() {
    send_mplayer_command("pause");
}

// 停止播放
void video_stop() {
    if (mplayer_pid > 0) {
        send_mplayer_command("stop");
        
        // 等待进程结束
        waitpid(mplayer_pid, NULL, 0);
        mplayer_pid = -1;
        
        // 删除FIFO
        unlink("/tmp/mplayer_fifo");
        
        // 停止进度更新定时器
        if (progress_timer) {
            lv_timer_del(progress_timer);
            progress_timer = NULL;
        }
        
        // 重置进度条
        lv_slider_set_value(ui_Slider3, 0, LV_ANIM_OFF);
        
        // 更新UI状态
        _ui_flag_modify(ui_stop, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        _ui_flag_modify(ui_stopbtn, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        _ui_flag_modify(ui_play, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_flag_modify(ui_playbtn, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
    }
}

// 快进10秒
void video_seek_forward() {
    send_mplayer_command("seek +3");
}

// 后退10秒
void video_seek_backward() {
    send_mplayer_command("seek -3");
}

// 跳转到指定位置
void video_seek(int percent) {
    if (mplayer_pid > 0) {
        char cmd[32];
        snprintf(cmd, sizeof(cmd), "seek %d 1", percent);
        send_mplayer_command(cmd);
    }
}

// 更新进度条定时器回调
void video_update_progress_timer(lv_timer_t *timer) {
    if (mplayer_pid > 0) {
        // 模拟进度更新（实际应用中需要解析mplayer输出）
        static int pos = 0;
        static int total = 300; // 5分钟视频
        
        if (pos < total) {
            pos += 1;
            int percent = (int)((float)pos / total * 100);
            lv_slider_set_value(ui_Slider3, percent, LV_ANIM_OFF);
        }
    }
}

// 辅助函数：检查文件扩展名是否有效
int is_valid_video_ext(const char *ext) {
    if (!ext) return 0;
    
    // 转换为小写进行比较
    char lower_ext[16] = {0};
    for (int i = 0; ext[i] && i < sizeof(lower_ext)-1; i++) {
        lower_ext[i] = tolower(ext[i]);
    }
    
    return (strcmp(lower_ext, ".mp4") == 0 ||
            strcmp(lower_ext, ".avi") == 0 ||
            strcmp(lower_ext, ".mkv") == 0 ||
            strcmp(lower_ext, ".mov") == 0);
}

// 加载视频文件到下拉菜单
void load_video_files() {
    if (!ui_Dropdown1) return;
    
    // 清空当前下拉菜单
    lv_dropdown_clear_options(ui_Dropdown1);
    
    // 打开视频目录
    DIR *dir = opendir(VIDEO_DIR);
    if (dir == NULL) {
        // 创建目录（如果不存在）
        mkdir(VIDEO_DIR, 0755);
        return;
    }
    
    // 遍历目录
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // 跳过隐藏文件和目录
        if (entry->d_name[0] == '.') continue;
        
        // 检查文件扩展名
        const char *ext = strrchr(entry->d_name, '.');
        if (ext && is_valid_video_ext(ext)) {
            // 添加到下拉菜单
            lv_dropdown_add_option(ui_Dropdown1, entry->d_name, LV_DROPDOWN_POS_LAST);
        }
    }
    closedir(dir);
}

// 下拉菜单事件处理
void video_dropdown_handler(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *dropdown = lv_event_get_target(e);
    
    if (code == LV_EVENT_VALUE_CHANGED) {
        char selected[128];
        lv_dropdown_get_selected_str(dropdown, selected, sizeof(selected));
        
        // 构建完整路径
        char full_path[256];
        snprintf(full_path, sizeof(full_path), "%s/%s", VIDEO_DIR, selected);
        
        // 播放选中的视频
        video_play(full_path);
    }
}

// 进度条事件处理
void video_progress_handler(lv_event_t *e) {
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
        if (mplayer_pid > 0) {
            int percent = lv_slider_get_value(ui_Slider3);
            video_seek(percent);
        }
    }
}

// 停止事件处理
void ui_event_stopbtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_PRESSED) {
        video_stop();
    }
}

// 播放事件处理
void ui_event_playbtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    
    if (event_code == LV_EVENT_PRESSED)
    {
        if (mplayer_pid > 0) 
        {
            // 播放/暂停切换
            video_toggle_pause();
        } else if (strlen(current_video_path) > 0) {
            // 重新播放当前视频
            video_play(current_video_path);
        } else {
            // 播放下拉框中选中的第一个视频
            char selected[128];
            lv_dropdown_get_selected_str(ui_Dropdown1, selected, sizeof(selected));
            if (strlen(selected) > 0) {
                char full_path[256];
                snprintf(full_path, sizeof(full_path), "%s/%s", VIDEO_DIR, selected);
                video_play(full_path);
            }
        }
    }
}

// 后退事件处理
void ui_event_backbtn(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_PRESSED) {
        video_seek_backward();
    }
}

// 快进事件处理
void ui_event_forwardbtn(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_PRESSED) {
        video_seek_forward();
    }
}