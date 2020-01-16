find_package(PkgConfig)
pkg_check_modules(PC_LibUSB QUIET libusb-1.0)

find_path(LibUSB_INCLUDE_DIR
    NAMES libusb.h
    PATHS ${PC_LibUSB_INCLUDE_DIRS})

find_library(LibUSB_LIBRARIES
    NAMES usb-1.0 libusb-1.0
    PATHS ${PC_LibUSB_LIBRARY_DIRS})

set(LibUSB_VERSION ${PC_LibUSB_VERSION})

mark_as_advanced(LibUSB_INCLUDE_DIR LibUSB_LIBRARIES)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibUSB
    REQUIRED_VARS LibUSB_INCLUDE_DIR
    VERSION_VAR LibUSB_VERSION)

if(LibUSB_FOUND)
    # Include parent directory for path-qualified includes
    get_filename_component(LibUSB_INCLUDE_PDIR ${LibUSB_INCLUDE_DIR} DIRECTORY)
endif()

if(LibUSB_FOUND AND NOT LibUSB::LibUSB)
    add_library(LibUSB::LibUSB SHARED IMPORTED)
    set_target_properties(LibUSB::LibUSB PROPERTIES
        IMPORTED_LOCATION "${LibUSB_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${LibUSB_INCLUDE_PDIR}")
endif()
