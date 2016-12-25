# Specify the minimum version for CMake

cmake_minimum_required(VERSION 3.6.2)

# Project's name

project(ipc)

set(SOURCE_FILES 
	src/populate.cc
	src/depopulate.cc
)
set(INCLUDE_PATH include)
set(EXPORTS_DIR ${CMAKE_BINARY_DIR}/exports)
set(EXPORTS_FILE_PATH ${EXPORTS_DIR}/ipc_export.h)

include(${CMAKE_INSTALL_PREFIX}/physics_config.cmake)
include(${CMAKE_INSTALL_PREFIX}/state_config.cmake)
include(${CMAKE_INSTALL_PREFIX}/player_config.cmake)

find_package(Threads REQUIRED)

set(LIBRARY_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/lib)
set(RUNTIME_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/bin)
set(INCLUDE_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/include)

add_library(ipc SHARED ${SOURCE_FILES})
generate_export_header(ipc EXPORT_FILE_NAME ${EXPORTS_FILE_PATH})
target_link_libraries(ipc state physics player Threads::Threads)
set_property(TARGET ipc PROPERTY CXX_STANDARD 11)
target_include_directories(ipc PUBLIC
	$<BUILD_INTERFACE:;${EXPORTS_DIR} ${INCLUDE_PATH}>
	$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
)
#export(TARGETS ipc FILE ipc_config.cmake)

install(TARGETS ipc EXPORT ipc_config
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)
install(EXPORT ipc_config DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY ${INCLUDE_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES ${EXPORTS_FILE_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
