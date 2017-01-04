# Specify the minimum version for CMake

cmake_minimum_required(VERSION 3.6.2)

# Project's name

project(ipc)

find_package(Protobuf REQUIRED)

include(${CMAKE_INSTALL_PREFIX}/physics_config.cmake)
include(${CMAKE_INSTALL_PREFIX}/state_config.cmake)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(include)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto/state.proto)

add_executable(ipc src/test.cpp src/depopulate.cpp src/populate.cpp ${PROTO_SRCS} ${PROTO_HDRS})
set_property(TARGET ipc PROPERTY CXX_STANDARD 11)
target_link_libraries(ipc ${Protobuf_LIBRARIES} physics state)

install(TARGETS ipc DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)