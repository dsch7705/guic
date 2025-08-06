set(SRC main.c
        gui.c
        gui_util.c
        gui_primatives.c
        gui_font.c
        gui_element.c)


add_executable(guic ${SRC})
target_compile_definitions(guic PRIVATE FONTS_DIR="${FONTS_DIR}")
if (USE_ASAN)
        target_compile_options(guic PRIVATE "-fsanitize=address")
        target_link_options(guic PRIVATE "-fsanitize=address")
endif()

target_include_directories(guic PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(guic PRIVATE SDL3::SDL3-static SDL3_ttf::SDL3_ttf-static)