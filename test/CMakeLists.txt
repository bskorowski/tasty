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


add_library(tastyTestCommon INTERFACE)
# Including private sources
# target_include_directories(tastyTestCommon INTERFACE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(tastyTestCommon INTERFACE tasty::tasty)

function(Register_Test name)
   add_executable(${name} src/${name}.cpp)
   target_link_libraries(${name} PRIVATE tastyTestCommon)
   target_compile_features(${name} PRIVATE cxx_std_23)
   add_test(NAME ${name} COMMAND ${name})
endfunction()


Register_Test(tasty_test)
Register_Test(runner_test)

# ---- End-of-file commands ----

add_folders(Test)
