cmake_minimum_required(VERSION 3.7)

# We use Boost.asio so we can later transition more easily to the C++ Networking TS library.

# Find all source files
file(GLOB_RECURSE VOLT_NET_SRCS
    "src/*.cpp"
    RELATIVE_PATH
)

file(GLOB_RECURSE VOLT_NET_HEADERS
    "include/*.hpp"
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

add_library(volt_net ${VOLT_NET_SRCS})
target_include_directories(volt_net PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")
target_link_libraries(volt_net PUBLIC ${CMAKE_THREAD_LIBS_INIT} Boost::boost volt_event)
set_property(TARGET volt_net PROPERTY CXX_STANDARD 17)

set_target_properties(volt_net PROPERTIES PUBLIC_HEADER "${VOLT_NET_HEADERS}")



include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")

write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

# Don't know how much we need this given this is a header-only library
install(
    TARGETS                     volt_net
    EXPORT                      "${TARGETS_EXPORT_NAME}"
    LIBRARY         DESTINATION "${CMAKE_INSTALL_LIBDIR}" # Probably not needed
    ARCHIVE         DESTINATION "${CMAKE_INSTALL_LIBDIR}" # Probably not needed
    RUNTIME         DESTINATION "${CMAKE_INSTALL_BINDIR}" # Probably not needed
    INCLUDES        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    # PUBLIC_HEADER   DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

# Headers
install(
    DIRECTORY "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/volt>"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    # FILES_MATCHING PATTERN "*.hpp"
)

# Configs
install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

# Config
#   * <prefix>/lib/cmake/Foo/FooTargets.cmake
install(
    EXPORT "${TARGETS_EXPORT_NAME}"
    # NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)