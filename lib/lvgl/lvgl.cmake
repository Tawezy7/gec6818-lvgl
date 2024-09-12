include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/
    ${CMAKE_CURRENT_LIST_DIR}/..
)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/core LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/display LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/draw LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/draw/nxp LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/draw/nxp/pxp LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/draw/nxp/vglite LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/draw/renesas LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/draw/renesas/dave2d LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/draw/sdl LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/draw/sw LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/arm2d LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/blend LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/blend/arm2d LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/blend/helium LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/blend/neon LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/draw/vg_lite LVGL_SOURSES)
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/drivers LVGL_SOURSES)
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/drivers/display LVGL_SOURSES)
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/drivers/display/drm LVGL_SOURSES)
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/drivers/display/fb LVGL_SOURSES)
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/drivers/display/ili9341 LVGL_SOURSES)
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/drivers/display/lcd LVGL_SOURSES)
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/drivers/display/renesas_glcdc LVGL_SOURSES)
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/drivers/display/st7735 LVGL_SOURSES)
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/drivers/display/st7789 LVGL_SOURSES)
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/drivers/display/st7796 LVGL_SOURSES)
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/drivers/display/tft_espi LVGL_SOURSES)
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/drivers/evdev LVGL_SOURSES)
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/drivers/glfw LVGL_SOURSES)
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/drivers/libinput LVGL_SOURSES)
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/drivers/nuttx LVGL_SOURSES)
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/drivers/qnx LVGL_SOURSES)
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/drivers/sdl LVGL_SOURSES)
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/drivers/wayland LVGL_SOURSES)
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/drivers/windows LVGL_SOURSES)
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/drivers/x11 LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/font LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/indev LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/layouts LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/layouts/flex LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/layouts/grid LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs/barcode LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs/bin_decoder LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs/bmp LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs/ffmpeg LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs/freetype LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs/fsdrv LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs/gif LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs/libjpeg_turbo LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs/libpng LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs/lodepng LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs/lz4 LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs/qrcode LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs/rle LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs/rlottie LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs/thorvg LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs/thorvg/rapidjson LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs/thorvg/rapidjson/error LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs/thorvg/rapidjson/internal LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs/thorvg/rapidjson/msinttypes LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs/tiny_ttf LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/libs/tjpgd LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/misc LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/misc/cache LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/osal LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/others LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/others/file_explorer LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/others/fragment LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/others/gridnav LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/others/ime LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/others/imgfont LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/others/monkey LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/others/observer LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/others/snapshot LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/others/sysmon LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/others/vg_lite_tvg LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/stdlib LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/stdlib/builtin LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/stdlib/clib LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/stdlib/micropython LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/stdlib/rtthread LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/themes LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/themes/default LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/themes/mono LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/themes/simple LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/tick LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/animimage LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/arc LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/bar LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/button LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/buttonmatrix LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/calendar LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/canvas LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/chart LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/checkbox LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/dropdown LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/image LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/imagebutton LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/keyboard LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/label LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/led LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/line LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/list LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/lottie LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/menu LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/msgbox LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/objx_templ LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/property LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/roller LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/scale LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/slider LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/span LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/spinbox LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/spinner LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/switch LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/table LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/tabview LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/textarea LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/tileview LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/widgets/win LVGL_SOURSES)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/demos
)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/demos LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/demos/benchmark LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/demos/benchmark/assets LVGL_SOURSES)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/demos LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/demos/widgets LVGL_SOURSES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/demos/widgets/assets LVGL_SOURSES)

