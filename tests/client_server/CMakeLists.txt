cmake_minimum_required(VERSION 3.5)

# Compile and run test
set(CMAKE_CXX_STANDARD 17)
add_executable(ts_client_server ${CMAKE_CURRENT_SOURCE_DIR}/client_server.cpp)
target_link_libraries(ts_client_server volt_net)

add_test(client_server_test ts_client_server)

# add_test(client_test client localhost 64420)
