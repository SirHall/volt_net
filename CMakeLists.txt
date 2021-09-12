cmake_minimum_required(VERSION 3.5)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/volt_hunt.cmake)

# set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/inst/") # For testing

project(volt_net VERSION 0.0.16)

include(ExternalProject)
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/dep)
include_directories(${EXTERNAL_INSTALL_LOCATION}/include)

# Add in volt_net
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/volt_net/)

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
