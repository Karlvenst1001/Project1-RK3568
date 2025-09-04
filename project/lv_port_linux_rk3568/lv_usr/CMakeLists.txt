# 添加源文件搜索路径,可以添加多个,并将所有源文件添加到CORE_SOURCES变量中
# 使用file(GLOB ...)可以查找当前目录下的所有源文件
# CONFIGURE_DEPENDS选项会在源文件变化时重新生成构建系统（CMake ≥ 3.12）
file(GLOB CORE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

add_library(lvgl_usr STATIC ${CORE_SOURCES}) # 创建静态库lvgl_usr

# 依赖核心库并设置头文件,将LVGL的头文件添加到lvgl_usr中
target_include_directories(lvgl_usr
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../lvgl
)

# 依赖核心库并设置头文件,将当前目录下的头文件添加到lvgl_usr中
target_include_directories(lvgl_usr
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

# 设置lvgl_usr库依赖于lvgl
target_link_libraries(lvgl_usr PRIVATE lvgl)