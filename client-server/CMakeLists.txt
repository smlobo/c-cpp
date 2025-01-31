cmake_minimum_required(VERSION 3.29)

project(client-server VERSION 1.0)

find_package(poco REQUIRED)

set(CMAKE_SKIP_RPATH TRUE)

add_executable(server src/server.cpp
        src/constants.h)

target_link_libraries(server PRIVATE Poco::Net Poco::Util)

