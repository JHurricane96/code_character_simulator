cmake_minimum_required(VERSION 3.6.2)
project(tester)

set(LIBSRC src/tester.cpp)
set(LIB_INCLUDE_PATH include)
set(LIB_EXPORTS_DIR ${CMAKE_BINARY_DIR}/exports)
set(LIB_EXPORTS_FILE_PATH ${LIB_EXPORTS_DIR}/tester_export.h)

set(LIBRARY_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/test/lib)
set(RUNTIME_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/test/bin)
set(INCLUDE_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/test/include)

add_library(tester SHARED ${LIBSRC})
generate_export_header(tester EXPORT_FILE_NAME ${LIB_EXPORTS_FILE_PATH})
target_include_directories(tester PUBLIC
	$<BUILD_INTERFACE:;${LIB_EXPORTS_DIR} ${LIB_INCLUDE_PATH}>
	$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/test/include>
)
#export(TARGETS tester FILE tester_config.cmake)

install(TARGETS tester EXPORT tester_config
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/test/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/test/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/test/bin
)
install(EXPORT tester_config DESTINATION ${CMAKE_INSTALL_PREFIX}/test)
install(DIRECTORY ${LIB_INCLUDE_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX}/test)
install(FILES ${LIB_EXPORTS_FILE_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX}/test/include)