cmake_minimum_required(VERSION 3.23.1)
project(lyon)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O3 -g -std=c++11")

find_package(yaml-cpp REQUIRED)
if(yaml-cpp_FOUND)
  include_directories(${YAML_CPP_INCLUDE_DIR})
endif()

set(
  LIB_SRC
  lyon/log.cc
  lyon/util.cc
  lyon/config.cc
)

add_library(lyon SHARED ${LIB_SRC})

add_executable(test_log tests/test_log.cc)
add_dependencies(test_log lyon)
target_link_libraries(test_log lyon yaml-cpp)


add_executable(test_config tests/test_config.cc)
add_dependencies(test_config lyon)
target_link_libraries(test_config lyon yaml-cpp)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
