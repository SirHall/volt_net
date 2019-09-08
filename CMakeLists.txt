cmake_minimum_required(VERSION 3.5)

project(volt_ge_net)

# Add in volt_net
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/volt_net/)

enable_testing()

# Add in tests
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests/client_server/)
