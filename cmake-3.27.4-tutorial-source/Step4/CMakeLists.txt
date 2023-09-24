# TODO 1: Update the minimum required version to 3.15

#cmake_minimum_required(VERSION 3.10)
cmake_minimum_required(VERSION 3.15)

# set the project name and version
project(Tutorial VERSION 1.0)

# specify the C++ standard
add_library(tutorial_compiler_flags INTERFACE)
target_compile_features(tutorial_compiler_flags INTERFACE cxx_std_11)

# TODO 2: Create helper variables to determine which compiler we are using:
# * Create a new variable gcc_like_cxx that is true if we are using CXX and
#   any of the following compilers: ARMClang, AppleClang, Clang, GNU, LCC
# * Create a new variable msvc_cxx that is true if we are using CXX and MSVC
# Hint: Use set() and COMPILE_LANG_AND_ID
message(STATUS "Sheldon: compiler id: ${CMAKE_CXX_COMPILER_ID}")
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "ARMClang" OR
        ${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang" OR
        ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR
        ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR
        ${CMAKE_CXX_COMPILER_ID} STREQUAL "LCC")
    set(gcc_like_cxx TRUE)
else ()
    set(gcc_like_cxx FALSE)
endif ()
set(gcc_like_cxx2 "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
message(STATUS "Sheldon: {gcc_like_cxx} ${gcc_like_cxx} / ${gcc_like_cxx2}")

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    set(msvc_cxx TRUE)
else ()
    set(msvc_cxx FALSE)
endif ()
set(msvc_cxx2 "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
message(STATUS "Sheldon: {msvc_cxx} ${msvc_cxx} / ${msvc_cxx2}")

# TODO 3: Add warning flag compile options to the interface library
# tutorial_compiler_flags.
# * For gcc_like_cxx, add flags -Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused
# * For msvc_cxx, add flags -W3
# Hint: Use target_compile_options()
#if (${gcc_like_cxx})
#    target_compile_options(tutorial_compiler_flags INTERFACE "-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused")
#elseif (${msvc_cxx})
#    target_compile_options(tutorial_compiler_flags INTERFACE "-W3")
#else ()
#    message(WARNING "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}")
#endif ()
#target_compile_options(tutorial_compiler_flags INTERFACE
#        "$<${gcc_like_cxx2}:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>"
#        "$<${msvc_cxx2}:-W3>"
#)

# TODO 4: With nested generator expressions, only use the flags for the
# build-tree
# Hint: Use BUILD_INTERFACE
target_compile_options(tutorial_compiler_flags INTERFACE
        "$<${gcc_like_cxx2}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
        "$<${msvc_cxx2}:$<BUILD_INTERFACE:-W3>>"
)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(TutorialConfig.h.in TutorialConfig.h)

# add the MathFunctions library
add_subdirectory(MathFunctions)

# add the executable
add_executable(Tutorial tutorial.cxx)

target_link_libraries(Tutorial PUBLIC MathFunctions tutorial_compiler_flags)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
