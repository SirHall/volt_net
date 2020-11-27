set(CONFIG_HUNT_DIR "${CMAKE_BINARY_DIR}/volt_hunt_repo.cmake")

if(NOT EXISTS ${CONFIG_HUNT_DIR})
    file(DOWNLOAD
        "https://raw.githubusercontent.com/SirHall/volt_hunt/master/volt_hunt_repo.cmake"
        ${CONFIG_HUNT_DIR}
        SHOW_PROGRESS
    )
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/HunterGate.cmake)

HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.284.tar.gz"
    SHA1 "b15e14b31d96ee2e2be32187d21e2390d0b974c8"
    FILEPATH "${CONFIG_HUNT_DIR}"
)
