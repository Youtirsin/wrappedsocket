cmake_minimum_required(VERSION 3.0.0)
project(WrappedSocket VERSION 0.1.0)

add_subdirectory("./wrappedsocket")

add_executable(server server.cpp)
add_executable(client client.cpp)
add_executable(test test.cpp)

target_link_libraries(server PRIVATE WrappedSocket)
target_link_libraries(client PRIVATE WrappedSocket)