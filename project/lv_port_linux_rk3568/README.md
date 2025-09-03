# LVGL for frame buffer device

LVGL configured to work with `/dev/fb0` on Linux, however it can be changed in [`lv_drv_conf.h`](https://github.com/lvgl/lv_port_linux_frame_buffer/blob/master/lv_drv_conf.h) by `FBDEV_PATH`.

When cloning this repository, also make sure to download submodules (`git submodule update --init --recursive`) otherwise you will be missing key components.

Check out this blog post for a step by step tutorial:
https://blog.lvgl.io/2018-01-03/linux_fb

## Clone the project

Clone the LVGL Framebuffer Demo project and its related sub modules:

```
git clone https://github.com/lvgl/lv_port_linux_frame_buffer.git
cd lv_port_linux_frame_buffer/
git submodule update --init --recursive
```

## Build the project

```
make
sudo make install
```

## Run the demo application

```
demo
```



> 基于LVGL的嵌入式车辆控制系统，实现灯光控制、蜂鸣器控制及时间显示功能

##  ✨ 功能亮点

- **灯光控制系统**：精确控制转向灯和双闪灯（GPIO控制）
- **灯光控制系统**：提供视频播放界面
- **灯光控制系统**：提供图库显示界面
- **实时北京时间**：高精度时间显示（误差<±1秒/月）
- **蜂鸣器反馈**：提供操作声音反馈
- **直观UI界面**：基于LVGL的现代化用户界面
- **状态互斥机制**：确保灯光控制逻辑安全可靠

##  🛠️ 硬件架构

| 组件 | 功能 | GPIO引脚 |
|------|------|----------|
| 左转向灯 | 车辆左转指示 | GPIO 120 |
| 右转向灯 | 车辆右转指示 | GPIO 124 |
| 双闪左灯 | 紧急情况指示 | GPIO 121 |
| 双闪右灯 | 紧急情况指示 | GPIO 123 |
| 蜂鸣器 | 操作反馈 | GPIO 111 |

##  📂 项目结构

```plaintext
项目结构说明（如上所示）
