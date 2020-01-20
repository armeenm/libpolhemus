cmake_minimum_required(VERSION 3.12...3.16)
project(libpolhemus
  VERSION 1.0.0
  DESCRIPTION "Library to interface with Polhemus sensors")

option(FORCE_COLORED_OUTPUT
  "Always produce ANSI-colored output (GNU/Clang only)" 
  OFF)

option(STATIC_ANALYSIS
  "Enable static analysis tools"
  OFF)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
find_package(LibUSB 1.0 REQUIRED MODULE)

################################################################################
### C++ API
################################################################################

file(GLOB CXX_SRCS
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS
  "polhemus/cxx/*.cpp")

add_library(polhemus_cc SHARED ${CXX_SRCS})
add_library(Polhemus::polhemus_cc ALIAS polhemus_cc)

target_include_directories(polhemus_cc
  PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(polhemus_cc PRIVATE -Wshadow -Wall -Wextra -pedantic -pipe)
target_compile_features(polhemus_cc
  PUBLIC
  cxx_std_17
  cxx_rvalue_references
  cxx_attributes
  cxx_noexcept
  cxx_deleted_functions
  cxx_defaulted_functions
  PRIVATE
  cxx_auto_type
  cxx_nonstatic_member_init
  cxx_uniform_initialization)
set_target_properties(polhemus_cc
  PROPERTIES
  EXPORT_NAME polhemus_cc
  CXX_EXTENSIONS OFF)

if(${FORCE_COLORED_OUTPUT})
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(polhemus_cc PRIVATE -fdiagnostics-color=always)
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(polhemus_cc PRIVATE -fcolor-diagnostics)
  endif()
endif()

if(${STATIC_ANALYSIS})
  set_target_properties(polhemus_cc PROPERTIES
    CXX_CLANG_TIDY "clang-tidy;-checks=*;-header-filter=.*"
    CXX_CPPCHECK "cppcheck;--std=c++11")
endif()

target_link_libraries(polhemus_cc PRIVATE LibUSB::LibUSB)

## Installation ##

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Polhemus)

install(TARGETS polhemus_cc
  EXPORT polhemus-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES include/polhemus.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

################################################################################
### C API
################################################################################

file(GLOB CAPI_SRCS
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS
  "polhemus/capi/*.cpp")

add_library(polhemus_c SHARED ${CAPI_SRCS})
add_library(Polhemus::polhemus ALIAS polhemus_c)

target_include_directories(polhemus_c
  PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(polhemus_c PRIVATE -Wshadow -Wall -Wextra -pedantic -pipe)
target_compile_features(polhemus_c
  PUBLIC
  c_function_prototypes
  PRIVATE
  cxx_std_17
  cxx_variadic_templates
  cxx_auto_type
  cxx_return_type_deduction
  cxx_decltype
  cxx_nullptr
  cxx_constexpr)
set_target_properties(polhemus_c
  PROPERTIES
  EXPORT_NAME polhemus
  CXX_EXTENSIONS OFF)

if(${FORCE_COLORED_OUTPUT})
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(polhemus_c PRIVATE -fdiagnostics-color=always)
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(polhemus_c PRIVATE -fcolor-diagnostics)
  endif()
endif()

if(${STATIC_ANALYSIS})
  set_target_properties(polhemus_c PROPERTIES
    CXX_CLANG_TIDY "clang-tidy;-checks=*;-header-filter=.*"
    CXX_CPPCHECK "cppcheck;--std=c++17")
endif()


target_link_libraries(polhemus_c PRIVATE polhemus_cc)

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Polhemus)

install(TARGETS polhemus_c
  EXPORT polhemus-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES include/polhemus.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

################################################################################
### Installation
################################################################################

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

install(FILES
  ${CMAKE_CURRENT_LIST_DIR}/cmake/FindLibUSB.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/PolhemusConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/PolhemusConfigVersion.cmake
  DESTINATION ${INSTALL_CONFIGDIR})

configure_file(${CMAKE_CURRENT_LIST_DIR}/cmake/FindLibUSB.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/FindPolhemus.cmake
  COPYONLY)

export(EXPORT polhemus-targets
  FILE 
  ${CMAKE_CURRENT_BINARY_DIR}/PolhemusTargets.cmake
  NAMESPACE 
  Polhemus::)

export(PACKAGE Polhemus)

#add_subdirectory(test)
