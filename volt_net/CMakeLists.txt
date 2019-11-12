cmake_minimum_required(VERSION 3.5)

# Set some default values
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CMAKE_CXX_STANDARD 17)

# Setup and include libraries
# We use Boost.asio so we can later transition more easily to the C++ Networking TS library.
# find_package(Boost COMPONENTS system filesystem REQUIRED)
# include_directories(${Boost_INCLUDE_DIR})

# Find all source files
file(GLOB_RECURSE SRCS
    "src/*.cpp"
    RELATIVE_PATH
)

find_package (Threads)

add_library(volt_net ${SRCS})
target_include_directories(volt_net PUBLIC ${INCLUDE_DIR})
target_link_libraries(volt_net ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(volt_net ${CONAN_LIBS})
# target_link_libraries(volt_net volt_event)
# target_link_libraries(volt_net LINK_PUBLIC ${Boost_LIBRARIES})

# SET(GXX_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
# Flags
# if(MSVC)
#     target_compile_options(volt_net PRIVATE /W4 /WX)
# else()

# set(WARNING_FLAGS "-Wall -Wextra -pedantic -Werror -Wno-unknown-pragmas -Wno-unused-parameter")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${WARNING_FLAGS} -g -fprofile-arcs -ftest-coverage")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${WARNING_FLAGS} -flto -O3")

# endif()