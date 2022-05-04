cmake_minimum_required(VERSION 3.23.1)
project(lyon)

include(cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O3 -g -std=c++11 -Wall")

find_package(yaml-cpp REQUIRED)
include_directories(${YAML_CPP_INCLUDE_DIR})

set(
  LIB_SRC
  lyon/log.cc
  lyon/util.cc
  lyon/config.cc
  lyon/thread.cc
  lyon/utils/file_system_util.cc
)


add_library(lyon SHARED ${LIB_SRC})
target_link_libraries(lyon PRIVATE yaml-cpp)

set(
  LIBS
  lyon
)

# add_executable(test_log tests/test_log.cc)
# add_dependencies(test_log lyon)
# target_link_libraries(test_log lyon yaml-cpp)
lyon_add_executable(test_log "tests/test_log.cc" lyon "${LIBS}")

lyon_add_executable(test_config "tests/test_config.cc" lyon "${LIBS}")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
