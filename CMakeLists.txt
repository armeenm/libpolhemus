cmake_minimum_required(VERSION 3.0)
project(libpolhemus)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_FLAGS "-Wshadow -Wall -Wextra -pedantic -pipe")

file(GLOB C_SRCS "src/*.c")
add_library(libpolhemus STATIC ${C_SRCS})
target_include_directories(libpolhemus PRIVATE src)
