cmake_minimum_required(VERSION 3.5)

# Set some default values
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CMAKE_CXX_STANDARD 17)

# Find all source files
file(GLOB_RECURSE SRCS
    "src/*.cpp"
    RELATIVE_PATH
)

find_package (Threads)

add_library(volt_net ${SRCS})
target_include_directories(volt_net PUBLIC ${INCLUDE_DIR})
target_link_libraries (volt_net ${CMAKE_THREAD_LIBS_INIT})

# Flags
if(MSVC)
    target_compile_options(volt_net PRIVATE /W4 /WX)
else()
    target_compile_options(volt_net PRIVATE -Wall -Wextra -pedantic -Werror -Wno-unknown-pragmas -Wno-unused-parameter)
endif()