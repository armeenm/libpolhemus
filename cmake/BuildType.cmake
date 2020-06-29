if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' by default")

  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "${default_build_type}")
endif()
