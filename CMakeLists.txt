cmake_minimum_required(VERSION 3.23.1)
project(lyon)

option(BUILD_TEST "ON for complile test" ON)

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
  lyon/mutex.cc
  lyon/fiber.cc
  lyon/scheduler.cc
  lyon/iomanager.cc
  lyon/timer.cc
  lyon/hook.cc
  lyon/utils/file_system_util.cc
)


add_library(lyon SHARED ${LIB_SRC})
target_link_libraries(lyon PRIVATE yaml-cpp)

set(
  LIBS
  lyon
)

if(BUILD_TEST)
  lyon_add_executable(test_log "tests/test_log.cc" lyon "${LIBS}")
  lyon_add_executable(test_config "tests/test_config.cc" lyon "${LIBS}")
  lyon_add_executable(test_fsutils "tests/test_fsutils.cc" lyon "${LIBS}")
  lyon_add_executable(test_util "tests/test_util.cc" lyon "${LIBS}")
  lyon_add_executable(test_thread "tests/test_thread.cc" lyon "${LIBS}")
  lyon_add_executable(test_fiber "tests/test_fiber.cc" lyon "${LIBS}")
  lyon_add_executable(test_scheduler "tests/test_scheduler.cc" lyon "${LIBS}")
  lyon_add_executable(test_iomanager "tests/test_iomanager.cc" lyon "${LIBS}")
  lyon_add_executable(test_hook "tests/test_hook.cc" lyon "${LIBS}")

endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
