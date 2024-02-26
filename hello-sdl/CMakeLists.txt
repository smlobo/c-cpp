cmake_minimum_required(VERSION 3.15)
project(hello-sdl C)

find_package(SDL2 REQUIRED CONFIG)

set(CMAKE_SKIP_RPATH TRUE)

add_executable(${PROJECT_NAME} src/main.c)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main SDL2::SDL2-static)

