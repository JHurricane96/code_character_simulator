cmake_minimum_required(VERSION 3.6.2)
project(state)

set(SOURCE_FILES
	src/state.cpp
	src/actor/actor.cpp
	src/actor/states/actor_idle_state.cpp
	src/actor/states/actor_dead_state.cpp
	src/actor/states/actor_path_planning_state.cpp
	src/actor/states/actor_attack_state.cpp
	src/actor/swordsman.cpp
	src/actor/base.cpp
	src/actor/flag.cpp
	src/actor/king.cpp
	src/actor/tower.cpp
	src/actor/fire_ball.cpp
	src/actor/magician.cpp
	src/actor/scout.cpp
	src/actor/projectile_handler.cpp
	src/terrain/terrain.cpp
	src/terrain/terrain_element.cpp
	src/path_planner/formation.cpp
	src/path_planner/graph.cpp
	src/path_planner/path_planner.cpp
	src/path_planner/path_planner_helper.cpp
	src/player_state_handler/player_state_handler.cpp
	src/player_state_handler/unit_view.cpp
)
set(INCLUDE_PATH include)
set(EXPORTS_DIR ${CMAKE_BINARY_DIR}/exports)
set(EXPORTS_FILE_PATH ${EXPORTS_DIR}/state_export.h)

if (NOT BUILD_ALL)
	include(${CMAKE_INSTALL_PREFIX}/physics_config.cmake)
endif()

set(LIBRARY_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/lib)
set(RUNTIME_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/bin)
set(INCLUDE_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/include)

add_library(state SHARED ${SOURCE_FILES})
target_link_libraries(state physics)
set_property(TARGET state PROPERTY CXX_STANDARD 11)
generate_export_header(state EXPORT_FILE_NAME ${EXPORTS_FILE_PATH})
target_include_directories(state PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_PATH}>
	$<BUILD_INTERFACE:${EXPORTS_DIR}>
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
