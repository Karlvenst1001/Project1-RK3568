#ifndef __VIDEO_H__
#define __VIDEO_H__

#include "../ui.h"

// 全局变量定义
pid_t mplayer_pid;
lv_timer_t *progress_timer;
char current_video_path[256];
const char *VIDEO_DIR;

// 发送命令到mplayer
void send_mplayer_command(const char *cmd);

// 播放视频
void video_play(const char *path);

// 暂停/恢复播放
void video_toggle_pause();

// 停止播放
void video_stop();

// 快进3秒
void video_seek_forward();

// 后退3秒
void video_seek_backward();

// 跳转到指定位置
void video_seek(int percent);

// 更新进度条定时器回调
void video_update_progress_timer(lv_timer_t *timer);

// 辅助函数：检查文件扩展名是否有效
int is_valid_video_ext(const char *ext);

// 加载视频文件到下拉菜单
void load_video_files();

// 下拉菜单事件处理
void video_dropdown_handler(lv_event_t *e);

// 进度条事件处理
void video_progress_handler(lv_event_t *e);

// 停止事件处理
void ui_event_stopbtn(lv_event_t * e);

// 播放事件处理
void ui_event_playbtn(lv_event_t * e);

// 后退事件处理
void ui_event_backbtn(lv_event_t *e);

// 快进事件处理
void ui_event_forwardbtn(lv_event_t *e);


#endif