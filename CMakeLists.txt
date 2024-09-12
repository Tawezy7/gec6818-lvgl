cmake_minimum_required(VERSION 3.22)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER /home/user/5.4.0/bin/arm-linux-gcc)
set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include(./lib/lvgl/lvgl.cmake)
include(./export/lvgl/port.cmake)

project(llvgl)

include_directories(./inc)
aux_source_directory(./src ALL_SOURCE)

add_executable(llvgl)

target_sources(llvgl PRIVATE
    ${ALL_SOURCE}
     ${LVGL_SOURSES}
)

add_custom_command(
    TARGET llvgl   # 目标的名称
    POST_BUILD              # 表示在目标构建完成后执行命令
    COMMAND ${CMAKE_COMMAND} -E copy ~/lvgl/build/llvgl /mnt/hgfs/share/llvgl # shell命令
)

target_link_options(llvgl PRIVATE "-lpthread")

# cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON 