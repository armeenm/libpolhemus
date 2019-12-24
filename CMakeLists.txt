cmake_minimum_required(VERSION 3.0)
project(polhemus)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "-Wshadow -Wall -Wextra -pedantic -pipe")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS}) 

# Library
file(GLOB CXX_SRCS "src/*.cpp")
add_library(polhemus SHARED ${CXX_SRCS})
target_include_directories(polhemus PRIVATE src include)
target_link_libraries(polhemus usb-1.0)
install(TARGETS polhemus DESTINATION lib)
install(FILES include/libpolhemus.h DESTINATION include)

# Testing
file(GLOB TST_C_SRCS "test/*.c")
add_executable(test ${TST_C_SRCS})
target_include_directories(test PRIVATE include)
target_link_libraries(test polhemus)
