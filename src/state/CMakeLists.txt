cmake_minimum_required(VERSION 3.6.2)
project(state)

set(SOURCE_FILES src/terrain/terrain.cpp src/terrain/terrain_element.cpp)
set(INCLUDE_PATH include)
set(EXPORTS_DIR ${CMAKE_BINARY_DIR}/exports)
set(EXPORTS_FILE_PATH ${EXPORTS_DIR}/state_export.h)

include(${CMAKE_INSTALL_PREFIX}/physics_config.cmake)

set(LIBRARY_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/lib)
set(RUNTIME_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/bin)
set(INCLUDE_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/include)

add_library(state SHARED ${SOURCE_FILES})
target_link_libraries(state physics)
set_property(TARGET state PROPERTY CXX_STANDARD 11)
generate_export_header(state EXPORT_FILE_NAME ${EXPORTS_FILE_PATH})
target_include_directories(state PUBLIC
	$<BUILD_INTERFACE:;${EXPORTS_DIR} ${INCLUDE_PATH}>
	$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
)
#export(TARGETS state FILE state_config.cmake)

install(TARGETS state EXPORT state_config
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)
install(EXPORT state_config DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY ${INCLUDE_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES ${EXPORTS_FILE_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
