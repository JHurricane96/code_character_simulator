cmake_minimum_required(VERSION 3.6.2)
project(main)

find_package(Threads REQUIRED)

if (NOT BUILD_ALL)
	include(${CMAKE_INSTALL_PREFIX}/physics_config.cmake)
	include(${CMAKE_INSTALL_PREFIX}/state_config.cmake)
	include(${CMAKE_INSTALL_PREFIX}/player_config.cmake)
	include(${CMAKE_INSTALL_PREFIX}/player1_config.cmake)
	include(${CMAKE_INSTALL_PREFIX}/player2_config.cmake)
	include(${CMAKE_INSTALL_PREFIX}/drivers_config.cmake)
endif()

set(RUNSRC main.cpp)
add_executable(main ${RUNSRC})
target_link_libraries(main physics state player1 player2 drivers)
set_property(TARGET main PROPERTY CXX_STANDARD 11)
install(TARGETS main DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
