cmake_minimum_required(VERSION 3.29)

project(curl-taojson-example VERSION 1.0)

find_package(CURL REQUIRED CONFIG)
find_package(taocpp-json REQUIRED)

set(CMAKE_SKIP_RPATH TRUE)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE CURL::libcurl taocpp::json)

