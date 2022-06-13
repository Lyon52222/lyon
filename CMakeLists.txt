cmake_minimum_required(VERSION 3.23.1)
project(lyon)

option(BUILD_TEST "ON for compile test" ON)
option(BUILD_EXAMPLES "ON for compile examples" ON)

include(cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O3 -g -std=c++20 -Wall")

find_package(yaml-cpp REQUIRED)
include_directories(${YAML_CPP_INCLUDE_DIR})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

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
  lyon/fdmanager.cc
  lyon/address.cc
  lyon/socket.cc
  lyon/ssl_socket.cc
  lyon/tcp_server.cc
  lyon/streams/stream.cc
  lyon/streams/socket_stream.cc
  lyon/serialize/bytearray.cc
  lyon/serialize/serializer.cc
  lyon/utils/file_system_util.cc
  lyon/http/http_protocol.cc
  lyon/http/http_parser.cc
  lyon/http/http_session.cc
  lyon/http/http_server.cc
  lyon/http/http_servlet.cc
  lyon/http/http_connection.cc
  lyon/rpc/rpc_protocol.cc
  lyon/rpc/rpc_method.cc
  lyon/rpc/rpc_result.cc
  lyon/rpc/rpc_session.cc
  lyon/rpc/rpc_server.cc
  lyon/rpc/rpc_client.cc
  lyon/rpc/rpc_register.cc
  lyon/rpc/rpc_connection_poll.cc
)

# 使用ragel解析src_rl文件并且将解析文件保存到outputdir下。
# 并将其添加到outputlist库中
# function(ragelmaker src_rl outputlist outputdir)
ragelmaker(lyon/http/http_request_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/lyon/http)
ragelmaker(lyon/http/http_response_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/lyon/http)
ragelmaker(lyon/uri.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/lyon)

add_library(lyon SHARED ${LIB_SRC})
target_link_libraries(
  lyon PRIVATE yaml-cpp
  ${OPENSSL_LIBRARIES}
)

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
  lyon_add_executable(test_address "tests/test_address.cc" lyon "${LIBS}")
  lyon_add_executable(test_uri "tests/test_uri.cc" lyon "${LIBS}")
  lyon_add_executable(test_socket "tests/test_socket.cc" lyon "${LIBS}")
  lyon_add_executable(test_bytearray "tests/test_bytearray.cc" lyon "${LIBS}")
  lyon_add_executable(test_serializer "tests/test_serializer.cc" lyon "${LIBS}")
  lyon_add_executable(test_http "tests/test_http.cc" lyon "${LIBS}")
  lyon_add_executable(test_http_parser "tests/test_http_parser.cc" lyon "${LIBS}")
  lyon_add_executable(test_tcp_server "tests/test_tcp_server.cc" lyon "${LIBS}")
  lyon_add_executable(test_http_server "tests/test_http_server.cc" lyon "${LIBS}")
  lyon_add_executable(test_http_connection "tests/test_http_connection.cc" lyon "${LIBS}")
  lyon_add_executable(test_traits "tests/test_traits.cc" lyon "${LIBS}")
  lyon_add_executable(test_rpc_server "tests/test_rpc_server.cc" lyon "${LIBS}")
  lyon_add_executable(test_rpc_client "tests/test_rpc_client.cc" lyon "${LIBS}")
  lyon_add_executable(test_rpc_register "tests/test_rpc_register.cc" lyon "${LIBS}")

endif()

if(BUILD_EXAMPLES)
  lyon_add_executable(example_echo_server "examples/example_echo_server.cc" lyon "${LIBS}")
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
