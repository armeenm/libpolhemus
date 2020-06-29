if(NOT CMAKE_EXPORT_COMPILE_COMMANDS)
  message(STATUS "Setting compile commands export to '${default_compile_commands_export}' by default")

  set_property(CACHE CMAKE_EXPORT_COMPILE_COMMANDS PROPERTY VALUE "${default_compile_commands_export}")
endif()
