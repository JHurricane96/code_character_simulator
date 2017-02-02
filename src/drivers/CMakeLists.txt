cmake_minimum_required(VERSION 3.6.2)
project(drivers)

set(SOURCE_FILES
	src/main_driver.cpp
	src/player_driver.cpp
)
set(INCLUDE_PATH include)
set(EXPORTS_DIR ${CMAKE_BINARY_DIR}/exports)
set(EXPORTS_FILE_PATH ${EXPORTS_DIR}/drivers_export.h)

if (NOT BUILD_ALL)
	include(${CMAKE_INSTALL_PREFIX}/physics_config.cmake)
	include(${CMAKE_INSTALL_PREFIX}/state_config.cmake)
	include(${CMAKE_INSTALL_PREFIX}/ipc_config.cmake)
	include(${CMAKE_INSTALL_PREFIX}/player_config.cmake)
endif()

find_package(Threads REQUIRED)

set(LIBRARY_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/lib)
set(RUNTIME_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/bin)
set(INCLUDE_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/include)

add_library(drivers SHARED ${SOURCE_FILES})
target_link_libraries(drivers state ipc physics player Threads::Threads)
set_property(TARGET drivers PROPERTY CXX_STANDARD 11)
generate_export_header(drivers EXPORT_FILE_NAME ${EXPORTS_FILE_PATH})
target_include_directories(drivers PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_PATH}>
	$<BUILD_INTERFACE:${EXPORTS_DIR}>
	$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
)
#export(TARGETS drivers FILE drivers_config.cmake)

install(TARGETS drivers EXPORT drivers_config
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)
install(EXPORT drivers_config DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY ${INCLUDE_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES ${EXPORTS_FILE_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
