cmake_minimum_required(VERSION 3.4.1)
project(blt4l)

add_subdirectory(subhook)

add_definitions(-D_GNU_SOURCE)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -fPIC -Og -ggdb3 -Wall -Wpedantic -Wextra -Wshadow -Wconversion -Wno-sign-compare")

include_directories(include subhook)

set(SOURCE_FILES
    src/hook.c
    )

add_library(blt_loader SHARED ${SOURCE_FILES} src/blt_main.c)

target_link_libraries(blt_loader subhook dl)
