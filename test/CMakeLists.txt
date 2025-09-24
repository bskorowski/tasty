cmake_minimum_required(VERSION 3.14)

project(tastyTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(tasty REQUIRED)
  enable_testing()
endif()

# ---- Tests ----

add_executable(tasty_test src/tasty_test.cpp)
target_link_libraries(tasty_test PRIVATE tasty::tasty)
target_compile_features(tasty_test PRIVATE cxx_std_23)

add_test(NAME tasty_test COMMAND tasty_test)

# ---- End-of-file commands ----

add_folders(Test)
