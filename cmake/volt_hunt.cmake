set(CONFIG_HUNT_DIR "${CMAKE_BINARY_DIR}/volt_hunt_repo.cmake")

if(NOT EXISTS ${CONFIG_HUNT_DIR})
    file(DOWNLOAD
        "http://fs.ovi.sh:8008"
        ${CONFIG_HUNT_DIR}
        SHOW_PROGRESS
    )
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/HunterGate.cmake)

HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.312.tar.gz"
    SHA1 "42311324f6b859983d833cffc2151d0b86b8bea4"
    FILEPATH "${CONFIG_HUNT_DIR}"
)
