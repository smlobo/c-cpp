cmake_minimum_required(VERSION 3.30)

project(emscripten_sdl)

add_definitions(-std=c++17)
set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} src/main.cpp
        src/sdl-square.cpp
)
