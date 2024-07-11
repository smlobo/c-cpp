cmake_minimum_required(VERSION 3.30)

project(hello-owl)

add_definitions(-std=c17)
set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} src/hello-owl.c)

target_compile_options(${PROJECT_NAME} PRIVATE -O2 -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS=["png"])

target_link_options(${PROJECT_NAME} PRIVATE -O2 -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS=["png"] --preload-file assets)

add_custom_command(TARGET ${PROJECT_NAME} PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_BINARY_DIR}/assets"
        DEPENDS assets
        VERBATIM ON
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/manual.html" "${CMAKE_BINARY_DIR}/manual.html"
        DEPENDS manual.html
        VERBATIM ON
)
