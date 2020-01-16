cmake_minimum_required(VERSION 3.12...3.16)
project(libpolhemus
    VERSION 1.0.0
    DESCRIPTION "Library to interface with Polhemus sensors")

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
find_package(LibUSB 1.0 REQUIRED MODULE)

################################################################################
### C++ API
################################################################################

file(GLOB POLHEMUS_CXX_SRCS
    LIST_DIRECTORIES false
    CONFIGURE_DEPENDS
    "src/cxx/*.cpp")

add_library(polhemus SHARED ${POLHEMUS_CXX_SRCS})
add_library(Polhemus::Polhemus ALIAS polhemus)

target_include_directories(polhemus
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cxx)


target_compile_options(polhemus PRIVATE -Wshadow -Wall -Wextra -pedantic -pipe)
target_compile_features(polhemus
    PUBLIC
        cxx_rvalue_references
        cxx_attributes
        cxx_noexcept
        cxx_deleted_functions
        cxx_defaulted_functions
    PRIVATE
        cxx_auto_type
        cxx_nonstatic_member_init
        cxx_uniform_initialization)
set_target_properties(polhemus PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(polhemus PRIVATE LibUSB::LibUSB)

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Polhemus)

install(TARGETS polhemus
    EXPORT polhemus-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES include/libpolhemus.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

################################################################################
### C API
################################################################################

file(GLOB POLHEMUS_CAPI_SRCS
    LIST_DIRECTORIES false
    CONFIGURE_DEPENDS
    "src/capi/*.cpp")

add_library(polhemus_c SHARED ${POLHEMUS_CAPI_SRCS})
add_library(Polhemus::CPolhemus ALIAS polhemus_c)

target_include_directories(polhemus_c
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/capi
        ${CMAKE_CURRENT_SOURCE_DIR}/src)

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
set_target_properties(polhemus_c PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(polhemus_c PRIVATE polhemus)

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Polhemus)

install(TARGETS polhemus_c
    EXPORT polhemus-c-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES include/libpolhemus.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

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
