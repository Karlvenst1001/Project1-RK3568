#ifndef __MUSIC_H__
#define __MUSIC_H__

#include "../ui.h"

// FIFO管道路径
#define MPLAYER_FIFO "/tmp/mplayer_fifo"

// 音乐播放控制变量
pid_t mplayer_pid; // mplayer进程ID
int is_playing; // 播放状态标志
int current_volume; // 当前音量(0-100)
char *current_song; // 当前播放歌曲
char **music_list; // 音乐文件列表
int music_count; // 音乐文件数量
int current_index; // 当前播放索引


void init_music_player();
void play_music(const char *filename);
void send_mplayer_music_command(const char *cmd);
void toggle_pause();
void stop_music();
void set_volume(int vol);
void play_next();
void play_prev();
void ui_event_Dropdownmusic(lv_event_t * e);
void ui_event_backmusicbtn(lv_event_t * e);
void ui_event_forwardmusicbtn(lv_event_t * e);
void ui_event_Slider6_value_changed(lv_event_t * e);
void ui_event_playmusicbtn(lv_event_t *e);
void ui_event_stopmusicbtn(lv_event_t *e);


#endif