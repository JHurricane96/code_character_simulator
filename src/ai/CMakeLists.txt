cmake_minimum_required(VERSION 3.6.2)
project(ai)

set(SOURCE_FILES
	src/ai.cpp
	src/attack_rules.cpp
	#src/guard_rules.cpp
)

set(INCLUDE_PATH include)
set(EXPORTS_DIR ${CMAKE_BINARY_DIR}/exports)
set(EXPORTS_FILE_PATH ${EXPORTS_DIR}/ai_export.h)

if (NOT BUILD_ALL)
	include(${CMAKE_INSTALL_PREFIX}/physics_config.cmake)
	include(${CMAKE_INSTALL_PREFIX}/state_config.cmake)
	include(${CMAKE_INSTALL_PREFIX}/player_config.cmake)
endif()

set(LIBRARY_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/lib)
set(RUNTIME_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/bin)
set(INCLUDE_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/include)

add_library(ai SHARED ${SOURCE_FILES})
target_link_libraries(ai state physics player)
set_property(TARGET ai PROPERTY CXX_STANDARD 11)
generate_export_header(ai EXPORT_FILE_NAME ${EXPORTS_FILE_PATH})
target_include_directories(ai PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_PATH}>
	$<BUILD_INTERFACE:${EXPORTS_DIR}>
	$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
)
#export(TARGETS ai FILE ai_config.cmake)

install(TARGETS ai EXPORT ai_config
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)
install(EXPORT ai_config DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY ${INCLUDE_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES ${EXPORTS_FILE_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
