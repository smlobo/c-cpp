cmake_minimum_required(VERSION 3.30)

project(naked-owl)

add_definitions(-std=c17)
set(CMAKE_C_STANDARD 17)

add_executable(${PROJECT_NAME} src/naked-owl.c)

target_compile_options(${PROJECT_NAME} PRIVATE -O2 -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS=["png"]
        -sUSE_SDL_TTF=2)

target_link_options(${PROJECT_NAME} PRIVATE -O2 -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS=["png"]
        -sUSE_SDL_TTF=2 -sEXPORTED_FUNCTIONS=["_mainf"] -sEXPORTED_RUNTIME_METHODS=["ccall"]
        --preload-file assets)

add_custom_command(TARGET ${PROJECT_NAME} PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_BINARY_DIR}/assets"
        DEPENDS assets
        VERBATIM ON
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/naked-owl.html" "${CMAKE_BINARY_DIR}/naked-owl.html"
        DEPENDS manual.html
        VERBATIM ON
)
