cmake_minimum_required(VERSION 3.5)

# Set some default values
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Setup and include libraries
# We use Boost.asio so we can later transition more easily to the C++ Networking TS library.
# find_package(Boost COMPONENTS system filesystem REQUIRED)
# include_directories(${Boost_INCLUDE_DIR})

# Find all source files
file(GLOB_RECURSE SRCS
    "src/*.cpp"
    RELATIVE_PATH
)

# Threading
find_package(Threads)

# Boost
hunter_add_package(Boost)
find_package(Boost CONFIG REQUIRED)

# volt_event
hunter_add_package(volt_event)
find_package(volt_event CONFIG REQUIRED)

add_library(volt_net ${SRCS})
target_include_directories(volt_net PUBLIC ${INCLUDE_DIR})
target_link_libraries(volt_net ${CMAKE_THREAD_LIBS_INIT} Boost::boost volt_event)
# target_link_libraries(volt_net ${CONAN_LIBS})
set_property(TARGET volt_net PROPERTY CXX_STANDARD 17)

# endif()
