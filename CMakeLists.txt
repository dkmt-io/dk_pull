cmake_minimum_required(VERSION 3.10)

project(dk_pull)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_CLANG_TIDY clang-tidy)

add_subdirectory(dk_pull/common)
add_subdirectory(dk_pull/event_loop)
