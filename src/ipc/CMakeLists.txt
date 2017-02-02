# Specify the minimum version for CMake

cmake_minimum_required(VERSION 3.6.2)

# Project's name

project(ipc)

set(SOURCE_FILES
	src/interrupts.cpp
	src/ipc.cpp
	src/load_terrain.cpp
	src/state_transfer.cpp
	src/store_terrain.cpp
)

set(INCLUDE_PATH include)
set(EXPORTS_DIR ${CMAKE_BINARY_DIR}/exports)
set(EXPORTS_FILE_PATH ${EXPORTS_DIR}/ipc_export.h)

find_package(Protobuf REQUIRED)

if (NOT BUILD_ALL)
	include(${CMAKE_INSTALL_PREFIX}/physics_config.cmake)
	include(${CMAKE_INSTALL_PREFIX}/state_config.cmake)
endif()

include_directories(${Protobuf_INCLUDE_DIRS})

file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/proto/*.proto")
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${ProtoFiles})

add_library(ipc SHARED ${SOURCE_FILES} ${PROTO_SRCS})
target_link_libraries(ipc ${Protobuf_LIBRARIES} physics state)
set_property(TARGET ipc PROPERTY CXX_STANDARD 11)
generate_export_header(ipc EXPORT_FILE_NAME ${EXPORTS_FILE_PATH})
target_include_directories(ipc PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_PATH}>
	$<BUILD_INTERFACE:${EXPORTS_DIR}>
	$<BUILD_INTERFACE:${Protobuf_INCLUDE_DIRS}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
	$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
)

install(TARGETS ipc EXPORT ipc_config
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)
install(EXPORT ipc_config DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY ${INCLUDE_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES ${EXPORTS_FILE_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(FILES ${PROTO_HDRS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
