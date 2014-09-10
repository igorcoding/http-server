cmake_minimum_required(VERSION 2.8.4)
project(http-server)

set(PROJECT_EXEC httpserver)

set (CMAKE_CXX_FLAGS "-std=c++11 -pthread")
set (CMAKE_EXE_LINKER_FLAGS "-std=c++11 -pthread")

set(SOURCE_FILES src/main.cc
			     src/http_server.cc
			     src/worker.cc
			     src/listener.cc
			     src/sck_t.cc
			     src/fs/file.cc
			     src/fs/file_error.cc
			     src/fs/file_reader.cc
			     src/http/header.cc
			     src/http/methods.cc
			     src/http/request.cc
			     src/http/response.cc
			     src/http/exceptions/malformed_components.cc
			     src/http/protocol.cc
			     src/config/server_config.cc
                 src/config/config_reader.cc
			     src/config/config_read_error.cc
			     src/util/thread_pool.cc
                 src/util/misc.cc
)

add_executable(${PROJECT_EXEC} ${SOURCE_FILES})

target_link_libraries (${PROJECT_EXEC} ev 
									   boost_system
									   boost_filesystem)