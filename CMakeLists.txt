cmake_minimum_required(VERSION 3.15...3.18 FATAL_ERROR)

list(PREPEND CMAKE_PREFIX_PATH
  ${CMAKE_BINARY_DIR})
list(PREPEND CMAKE_MODULE_PATH
  ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(default_build_type "Release")
include(cmake/BuildType.cmake)

project(polhemus
  VERSION 1.0.0
  DESCRIPTION "Library to interface with polhemus sensors")

option(FORCE_COLORED_OUTPUT
  "Always produce ANSI-colored output (GNU/Clang only)" 
  OFF)

option(STATIC_ANALYSIS
  "Enable static analysis tools"
  OFF)

option(C_BINDINGS
  "Enable C bindings"
  ON)

if(FORCE_COLORED_OUTPUT OR CMAKE_GENERATOR STREQUAL "Ninja")
  add_compile_options(
    $<$<CXX_COMPILER_ID:GNU>:-fdiagnostics-color=always>
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-fcolor-diagnostics>)
endif()

set(default_compile_commands_export ON)
include(cmake/ExportCompileCommands.cmake)

find_package(libusb REQUIRED CONFIG)
find_package(fmt REQUIRED CONFIG)

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/polhemus)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

set(BUILD_OPTS 
  $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
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
    $<$<CONFIG:Debug>:
      -fsanitize=address
      -fsanitize=undefined
    >
    $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:MinSizeRel>>:
      -flto
    >
  >
  $<$<CXX_COMPILER_ID:MSVC>:
    /W4
  >)

################################################################################
### C++ Library
################################################################################

file(GLOB CXX_SRCS
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/cxx/*.cpp")

add_library(polhemus_cxx SHARED ${CXX_SRCS})
add_library(polhemus::polhemus_cxx ALIAS polhemus_cxx)

target_include_directories(polhemus_cxx
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_options(polhemus_cxx PRIVATE ${BUILD_OPTS})
target_compile_features(polhemus_cxx PUBLIC cxx_std_17)
set_target_properties(polhemus_cxx PROPERTIES
  EXPORT_NAME
    polhemus_cxx

  $<STATIC_ANALYSIS:
    CXX_CLANG_TIDY
      "clang-tidy;-checks=*;-header-filter=.*"
    CXX_CPPCHECK
      "cppcheck;--std=c++11"
  >)

target_link_libraries(polhemus_cxx
  PRIVATE
    debug "$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:asan;ubsan>"
    fmt::fmt
    libusb::libusb)

################
## Installation
################

install(TARGETS polhemus_cxx
  EXPORT
    polhemus-targets
  LIBRARY DESTINATION
    ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION
    ${CMAKE_INSTALL_LIBDIR})

install(
  FILES
    include/polhemus.hpp
  DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR})

################################################################################
### C Bindings
################################################################################

if(C_BINDINGS)
  file(GLOB C_SRCS
    LIST_DIRECTORIES
      false
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/capi/*.cpp")

  add_library(polhemus_c SHARED ${C_SRCS})
  add_library(polhemus::polhemus_c ALIAS polhemus_c)

  target_include_directories(polhemus_c
    PUBLIC
      $<INSTALL_INTERFACE:include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/src)

  target_compile_options(polhemus_c PRIVATE ${BUILD_OPTS})
  target_compile_features(polhemus_c PUBLIC c_function_prototypes PRIVATE cxx_std_17)

  set_target_properties(polhemus_c PROPERTIES
    EXPORT_NAME
      polhemus_c

    $<STATIC_ANALYSIS:
      CXX_CLANG_TIDY
        "clang-tidy;-checks=*;-header-filter=.*"
      CXX_CPPCHECK
        "cppcheck;--std=c++11"
    >)

  target_link_libraries(polhemus_c
    PUBLIC
      polhemus_cxx
    PRIVATE
      fmt::fmt
      libusb::libusb
      debug "$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:asan;ubsan>")

  ################
  ## Installation
  ################

  install(TARGETS polhemus_c
    EXPORT
      polhemus-targets
    LIBRARY DESTINATION
      ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION
      ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION
      ${CMAKE_INSTALL_BINDIR})

  install(
    FILES
      include/polhemus.h
    DESTINATION
      ${CMAKE_INSTALL_INCLUDEDIR})

endif()

################################################################################
### Installation
################################################################################

install(EXPORT polhemus-targets
  FILE
    polhemusTargets.cmake
  NAMESPACE
    polhemus::
  DESTINATION
    ${INSTALL_CONFIGDIR})

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/polhemusConfigVersion.cmake
  VERSION
    ${PROJECT_VERSION}
  COMPATIBILITY
    AnyNewerVersion)

configure_package_config_file(
  ${CMAKE_CURRENT_LIST_DIR}/cmake/polhemusConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/polhemusConfig.cmake
  INSTALL_DESTINATION
    ${INSTALL_CONFIGDIR})

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/polhemusConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/polhemusConfigVersion.cmake
  DESTINATION
    ${INSTALL_CONFIGDIR})

export(EXPORT polhemus-targets
  FILE 
    ${CMAKE_CURRENT_BINARY_DIR}/polhemusTargets.cmake
  NAMESPACE 
    polhemus::)

export(PACKAGE polhemus)
