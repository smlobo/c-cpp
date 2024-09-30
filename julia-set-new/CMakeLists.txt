cmake_minimum_required(VERSION 3.30)

project(julia-set)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED CONFIG)

set(CMAKE_SKIP_RPATH TRUE)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/julia-set.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main SDL2::SDL2-static)
