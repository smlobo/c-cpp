cmake_minimum_required(VERSION 3.15)

project(map-routing VERSION 1.0)

find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_TTF REQUIRED CONFIG)

set(CMAKE_SKIP_RPATH TRUE)

add_executable(${PROJECT_NAME}
        src/draw.cpp
        src/Graph.cpp
        src/Node.cpp
        src/Edge.cpp
        src/SDL-Helpers.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main SDL2::SDL2-static SDL2_ttf::SDL2_ttf-static)

