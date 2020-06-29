if(FORCE_COLORED_OUTPUT OR CMAKE_GENERATOR STREQUAL "Ninja")
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)

  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-fcolor-diagnostics)
  endif()
endif()
