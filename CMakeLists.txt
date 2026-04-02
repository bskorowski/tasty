cmake_minimum_required(VERSION 3.14)


project(
    tasty
    VERSION 0.1.0
    DESCRIPTION "C++ Testing library"
    HOMEPAGE_URL " "
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)


add_library(
    tasty_tasty
    INTERFACE
    include/tasty/tasty.hpp
    include/tasty/errors.hpp
    include/tasty/runners.hpp
)
add_library(tasty::tasty ALIAS tasty_tasty)

set_property(
    TARGET tasty_tasty PROPERTY
    EXPORT_NAME tasty
)

target_include_directories(
    tasty_tasty ${warning_guard}
    INTERFACE
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)


# std::print on windows mingw
if(WIN32 AND MINGW)
   target_link_libraries(tasty_tasty INTERFACE stdc++exp)
endif()


# TODO :: maybe vendor it
include(FetchContent)
FetchContent_Declare(
   rainbowcpp
   GIT_REPOSITORY https://github.com/oosiriiss/rainbowcpp
   GIT_TAG 0.2
)
FetchContent_MakeAvailable(rainbowcpp)
target_link_libraries(tasty_tasty INTERFACE rainbowcpp::rainbowcpp)


target_compile_features(tasty_tasty INTERFACE cxx_std_23)



# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
   option(BUILD_EXAMPLES "Build examples tree." ON)
  if(BUILD_EXAMPLES)
    add_subdirectory(example)
  endif()
endif()
