cmake_minimum_required(VERSION 3.6.2)
project(main)

include(${CMAKE_INSTALL_PREFIX}/test/tester_config.cmake)
set(RUNSRC main.cpp)
add_executable(main ${RUNSRC})
target_link_libraries(main tester)
install(TARGETS main DESTINATION ${CMAKE_INSTALL_PREFIX}/test/bin)
