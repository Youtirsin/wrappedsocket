cmake_minimum_required(VERSION 3.0.0)
project(WrappedSocket VERSION 0.1.0)

add_library(WrappedSocket STATIC wrappedsocket.cpp)
target_include_directories(WrappedSocket PUBLIC .)

add_executable(server tests/server.cpp)
target_link_libraries(server PRIVATE WrappedSocket)

add_executable(client tests/client.cpp)
target_link_libraries(client PRIVATE WrappedSocket)

add_executable(test tests/test.cpp)
