cmake_minimum_required(VERSION 3.1)

#指定编译器,必须在project()之前
set(CMAKE_SYSTEM_NAME Linux)    # 目标系统是 Linux
set(CMAKE_SYSTEM_PROCESSOR aarch64)   # 目标 CPU 是 ARM64（aarch64）
set(CMAKE_C_COMPILER "/bin/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc")    #c编译器
set(CMAKE_CXX_COMPILER "/bin/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-g++")  #c++编译器

project(lvgl_fb)

include_directories(.)
add_subdirectory(lvgl)
add_subdirectory(lv_drivers)
add_subdirectory(lv_usr)
add_subdirectory(ui)

add_executable(${PROJECT_NAME} main.c mouse_cursor_icon.c)
target_link_libraries(${PROJECT_NAME} PRIVATE lvgl lvgl::examples lvgl::demos lvgl::drivers lvgl_usr ui)
