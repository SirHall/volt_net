cmake_minimum_required(VERSION 3.5)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/HunterGate.cmake)

HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.284.tar.gz"
    SHA1 "b15e14b31d96ee2e2be32187d21e2390d0b974c8"
    FILEPATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Hunter/config.cmake"
)

# set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/inst/") # For testing

project(volt_net VERSION 0.0.1)

# Setup Conan
# include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
# conan_basic_setup()
# message(STATUS "Libs: ${CONAN_LIBS}")

# Add in volt_net
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/volt_net/)

# Add in volt_event
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dep/volt_event/)

#Setup Doxygen
# find_package(Doxygen)
# if (DOXYGEN_FOUND)
#     set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
#     set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

#     configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
#     message("Doxygen build started")

#     add_custom_target( doc_doxygen_volt_net ALL
#         COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
#         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#         COMMENT "Generating API documentation with Doxygen"
#         VERBATIM )
# else (DOXYGEN_FOUND)
#     message("Doxygen need to be installed to generate the doxygen documentation")
# endif ()

enable_testing()

# Add in examples
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests/client_server/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests/single_server/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests/single_client/)
