cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    tasty
    VERSION 0.1.0
    DESCRIPTION "C++ Testing library"
    HOMEPAGE_URL " "
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    tasty_tasty
    src/tasty.cpp
    include/tasty/tasty.hpp
    include/tasty/errors.hpp
    include/tasty/runners.hpp
)
add_library(tasty::tasty ALIAS tasty_tasty)

include(GenerateExportHeader)
generate_export_header(
    tasty_tasty
    BASE_NAME tasty
    EXPORT_FILE_NAME export/tasty/tasty_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(tasty_tasty PUBLIC TASTY_STATIC_DEFINE)
endif()

set_target_properties(
    tasty_tasty PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME tasty
    OUTPUT_NAME tasty
)

target_include_directories(
    tasty_tasty ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    tasty_tasty SYSTEM
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(tasty_tasty PUBLIC cxx_std_23)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_EXAMPLES "Build examples tree." "${tasty_DEVELOPER_MODE}")
  if(BUILD_EXAMPLES)
    add_subdirectory(example)
  endif()
endif()

# ---- Developer mode ----

if(NOT tasty_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of tasty"
  )
endif()

include(cmake/dev-mode.cmake)
