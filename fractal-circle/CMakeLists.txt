cmake_minimum_required(VERSION 3.15)

project(fractal-circle VERSION 1.0)

find_package(SDL2 REQUIRED CONFIG)

set(CMAKE_SKIP_RPATH TRUE)

add_executable(${PROJECT_NAME} src/main.cpp
        src/fractal-circle.cpp
        src/utilities.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main SDL2::SDL2-static)

