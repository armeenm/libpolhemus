cmake_minimum_required(VERSION 3.0)
project(polhemus)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "-Wshadow -Wall -Wextra -pedantic -pipe")
set(CMAKE_C_FLAGS "-Wshadow -Wall -Wextra -pedantic -pipe")

# Library
file(GLOB CXX_SRCS "src/*.cpp")
add_library(polhemus STATIC ${CXX_SRCS})
target_include_directories(polhemus PRIVATE src)
target_link_libraries(polhemus usb-1.0)

# Testing
file(GLOB TST_C_SRCS "test/*.c")
add_executable(test ${TST_C_SRCS})
target_include_directories(test PRIVATE include)
target_link_libraries(test polhemus)
