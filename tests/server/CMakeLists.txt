cmake_minimum_required(VERSION 3.5)

# Compile and run test
set(CMAKE_CXX_STANDARD 17)
add_executable(server ${CMAKE_CURRENT_SOURCE_DIR}/server.cpp)
target_link_libraries(server volt_net)