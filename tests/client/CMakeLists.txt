cmake_minimum_required(VERSION 3.5)

# Compile and run test
set(CMAKE_CXX_STANDARD 17)
add_executable(client ${CMAKE_CURRENT_SOURCE_DIR}/client.cpp)
target_link_libraries(client volt_net)
