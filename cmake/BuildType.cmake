if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' by default")

  set(
    CMAKE_BUILD_TYPE "${default_build_type}"
    CACHE
    STRING
      "Choose the build type"
    FORCE)

  set_property(
    CACHE
      CMAKE_BUILD_TYPE
    PROPERTY STRINGS
      "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
