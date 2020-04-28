cmake_minimum_required(VERSION 3.15...3.17 FATAL_ERROR)

include(cmake/BuildType.cmake)
include(cmake/ExportCompileCommands.cmake)

project(polhemus
  VERSION 1.0.0
  DESCRIPTION "Library to interface with Polhemus sensors")

option(FORCE_COLORED_OUTPUT
  "Always produce ANSI-colored output (GNU/Clang only)" 
  OFF)

option(STATIC_ANALYSIS
  "Enable static analysis tools"
  OFF)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(LibUSB 1.0 REQUIRED MODULE)

set(BUILD_OPTS 
  -Wshadow
  -Warray-bounds
  -Wstringop-overflow
  -Wduplicated-cond
  -Wlogical-op
  -Wduplicated-branches
  -Wnull-dereference
  -Wold-style-cast
  -Wuseless-cast
  -Wformat=2 
  -Wall
  -Wextra
  -Wconversion
  -Wpedantic
  -pipe
  -flto)

file(GLOB CXX_SRCS
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/polhemus/cxx/*.cpp")

file(GLOB C_SRCS
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/polhemus/capi/*.cpp")

add_library(polhemus_cxx SHARED ${CXX_SRCS})
add_library(polhemus_c SHARED ${C_SRCS})

add_library(Polhemus::polhemus_cxx ALIAS polhemus_cxx)
add_library(Polhemus::polhemus_c ALIAS polhemus_c)

target_include_directories(polhemus_cxx
  PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(polhemus_c
  PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(polhemus_cxx PRIVATE ${BUILD_OPTS})
target_compile_options(polhemus_c PRIVATE ${BUILD_OPTS})

target_compile_features(polhemus_cxx PUBLIC cxx_std_17)
target_compile_features(polhemus_c PUBLIC c_function_prototypes PRIVATE cxx_std_17)

set_target_properties(polhemus_cxx PROPERTIES EXPORT_NAME polhemus_cxx)
set_target_properties(polhemus_c PROPERTIES EXPORT_NAME polhemus_c)

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
  target_compile_options(polhemus_cxx PRIVATE -fsanitize=address -fsanitize=undefined)
  target_compile_options(polhemus_c PRIVATE -fsanitize=address -fsanitize=undefined)

  target_link_libraries(polhemus_cxx PRIVATE asan ubsan)
  target_link_libraries(polhemus_c PRIVATE asan ubsan)
endif()

if(FORCE_COLORED_OUTPUT OR CMAKE_GENERATOR STREQUAL "Ninja")
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(polhemus_cxx PRIVATE -fdiagnostics-color=always)
    target_compile_options(polhemus_c PRIVATE -fdiagnostics-color=always)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(polhemus_cxx PRIVATE -fcolor-diagnostics)
    target_compile_options(polhemus_c PRIVATE -fcolor-diagnostics)
  endif()
endif()

if(STATIC_ANALYSIS)
  set_target_properties(polhemus_cxx PROPERTIES
    CXX_CLANG_TIDY "clang-tidy;-checks=*;-header-filter=.*"
    CXX_CPPCHECK "cppcheck;--std=c++11")

  set_target_properties(polhemus_c PROPERTIES
    CXX_CLANG_TIDY "clang-tidy;-checks=*;-header-filter=.*"
    CXX_CPPCHECK "cppcheck;--std=c++11")
endif()

target_link_libraries(polhemus_cxx PRIVATE LibUSB::LibUSB)
target_link_libraries(polhemus_c PRIVATE polhemus_cxx)

################################################################################
### Installation
################################################################################

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Polhemus)

install(TARGETS polhemus_cxx
  EXPORT polhemus-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(TARGETS polhemus_c
  EXPORT polhemus-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES include/polhemus.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES include/polhemus.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT polhemus-targets
  FILE
  PolhemusTargets.cmake
  NAMESPACE
  Polhemus::
  DESTINATION
  ${INSTALL_CONFIGDIR})

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/PolhemusConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/PolhemusConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/PolhemusConfig.cmake
  INSTALL_DESTINATION ${INSTALL_CONFIGDIR})

configure_file(
  ${CMAKE_CURRENT_LIST_DIR}/cmake/FindLibUSB.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/FindLibUSB.cmake
  COPYONLY)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/FindLibUSB.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/PolhemusConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/PolhemusConfigVersion.cmake
  DESTINATION ${INSTALL_CONFIGDIR})

export(EXPORT polhemus-targets
  FILE 
  ${CMAKE_CURRENT_BINARY_DIR}/PolhemusTargets.cmake
  NAMESPACE 
  Polhemus::)

export(PACKAGE Polhemus)
