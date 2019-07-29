cmake_minimum_required(VERSION 3.5)

project(server)
project(client)

add_compile_options(-Wall -Wextra -Wpedantic -Wno-unknown-pragmas -Wno-unused-parameter -Iinclude -std=c++17 -g -rdynamic)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE SRCS
    #[["*.hpp"]]
    "src/*.cpp"
    RELATIVE_PATH
)

find_package (Threads)


add_executable(server tests/server/server.cpp ${SRCS})
target_include_directories(server PUBLIC ${INCLUDE_DIR})
target_link_libraries (server ${CMAKE_THREAD_LIBS_INIT})


add_executable(client tests/client/client.cpp ${SRCS})
target_include_directories(client PUBLIC ${INCLUDE_DIR})
target_link_libraries (client ${CMAKE_THREAD_LIBS_INIT})
