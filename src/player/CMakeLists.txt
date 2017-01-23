cmake_minimum_required(VERSION 3.6.2)
project(player)

set(SOURCE_FILES
	src/player_ai.cpp
)
set(INCLUDE_PATH include)
set(EXPORTS_DIR ${CMAKE_BINARY_DIR}/exports)
set(EXPORTS_FILE_PATH ${EXPORTS_DIR}/player_export.h)

if (NOT BUILD_ALL)
	include(${CMAKE_INSTALL_PREFIX}/physics_config.cmake)
	include(${CMAKE_INSTALL_PREFIX}/state_config.cmake)
endif()

set(LIBRARY_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/lib)
set(RUNTIME_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/bin)
set(INCLUDE_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/include)

add_library(player SHARED ${SOURCE_FILES})
target_link_libraries(player state physics)
set_property(TARGET player PROPERTY CXX_STANDARD 11)
generate_export_header(player EXPORT_FILE_NAME ${EXPORTS_FILE_PATH})
target_include_directories(player PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_PATH}>
	$<BUILD_INTERFACE:${EXPORTS_DIR}>
	$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
)
#export(TARGETS player FILE player_config.cmake)

install(TARGETS player EXPORT player_config
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)
install(EXPORT player_config DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY ${INCLUDE_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES ${EXPORTS_FILE_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
