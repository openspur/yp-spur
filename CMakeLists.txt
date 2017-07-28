cmake_minimum_required(VERSION 3.0.2)
project(yp-spur
	VERSION 1.14.0
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)
message(${CMAKE_MODULE_PATH})

set(YP_VENDOR_NAME "OpenSpur.org")
set(YP_PRODUCT_NAME "Yamabico Project - Spur")
set(YP_PROTOCOL_NAME "YPP:09:03")
set(YP_PARAMS_DIR "robot-params")

option(USE_SSM "Compile with SSM if available." ON)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
include(FindPkgConfig)
if(USE_SSM)
	find_package(SSM)
endif(USE_SSM)

configure_file(${CMAKE_SOURCE_DIR}/cmake/config.h.cmake
	${CMAKE_CURRENT_BINARY_DIR}/include/config.h
)
add_definitions(-DHAVE_CONFIG_H=1)


add_subdirectory(auxlib)
include_directories(
	include
	${CMAKE_CURRENT_BINARY_DIR}/include
	auxlib
	auxlib/formula-calc/include
)


# Build coordinator and utilities

add_executable(ypspur-coordinator
	src/adinput.c
	src/command_ad.c
	src/command.c
	src/command_get.c
	src/command_joint.c
	src/command_param.c
	src/command_run.c
	src/command_set.c
	src/communication.c
	src/control_motion.c
	src/control_vehicle.c
	src/ipcommunication.c
	src/msq.win32.c
	src/odometry.c
	src/param.c
	src/serial.c
	src/ssm_spur_handler.c
	src/utility.c
	src/ypprotocol.c
	src/yprintf.c
	src/ypspur-coordinator.c
)
target_link_libraries(ypspur-coordinator
	carte2d m formula-calc Threads::Threads
)
if(SSM_FOUND)
	target_link_libraries(ypspur-coordinator
		odomssm
	)
endif(SSM_FOUND)
if(WIN32)
	target_link_libraries(ypspur-coordinator wsock32 ws2_32)
endif(WIN32)

add_executable(ypspur-free
	src/ypspur-free.c
)
target_link_libraries(ypspur-free
	ypspur
)

add_executable(ypspur-interpreter
	src/ypspur-interpreter.c
)

target_link_libraries(ypspur-interpreter
	ypspur
)


# Build libraries

add_library(ypspur SHARED
	src/libypspur.c
	src/libypspur-md.c
	src/ipcommunication.c
	src/msq.win32.c
	src/yprintf.c
	src/utility.c
)
if(WIN32)
	target_link_libraries(ypspur wsock32 ws2_32)
endif(WIN32)
SET_TARGET_PROPERTIES(ypspur PROPERTIES IMPORT_SUFFIX "_import.lib")

add_library(ypspur-static STATIC
	src/libypspur.c
	src/libypspur-md.c
	src/ipcommunication.c
	src/msq.win32.c
	src/yprintf.c
	src/utility.c
)
if(WIN32)
	target_link_libraries(ypspur-static wsock32 ws2_32)
endif(WIN32)
SET_TARGET_PROPERTIES(ypspur-static PROPERTIES OUTPUT_NAME "ypspur")

add_library(ypspur-md SHARED
	src/libypspur-md.c
	src/ipcommunication.c
	src/msq.win32.c
	src/yprintf.c
	src/utility.c
)
if(WIN32)
	target_link_libraries(ypspur-md wsock32 ws2_32)
endif(WIN32)
SET_TARGET_PROPERTIES(ypspur-md PROPERTIES IMPORT_SUFFIX "_import.lib")

add_library(ypspur-md-static STATIC
	src/libypspur-md.c
	src/ipcommunication.c
	src/msq.win32.c
	src/yprintf.c
	src/utility.c
)
if(WIN32)
	target_link_libraries(ypspur-md-static wsock32 ws2_32)
endif(WIN32)
SET_TARGET_PROPERTIES(ypspur-md-static PROPERTIES OUTPUT_NAME "ypspur-md")


# Install

install(TARGETS
		ypspur-coordinator
		ypspur
		ypspur-md
		ypspur-free
		ypspur-interpreter
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
)
install(FILES
		include/ypspur.h
		include/ypspur-md.h
		include/ypparam.h
	DESTINATION include
)
if(SSM_FOUND)
	install(DIRECTORY
			include/ssmtype
		DESTINATION include
	)
endif(SSM_FOUND)

