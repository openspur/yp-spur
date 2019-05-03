cmake_minimum_required(VERSION 2.8.3)

set(_YPSPUR_MAJOR_VERSION 1)
set(_YPSPUR_MINOR_VERSION 16)
set(_YPSPUR_PATCH_VERSION 0)

if(${CMAKE_MAJOR_VERSION} LESS 3)
# DARN_IT: CMake 2 should be supported until April, 2019 (Ubuntu Trusty EOL)
  cmake_policy(VERSION 2.8.3)
  project(ypspur)

  set(YPSPUR_MAJOR_VERSION ${_YPSPUR_MAJOR_VERSION})
  set(YPSPUR_MINOR_VERSION ${_YPSPUR_MINOR_VERSION})
  set(YPSPUR_PATCH_VERSION ${_YPSPUR_PATCH_VERSION})

  set(PROJECT_VERSION ${_YPSPUR_MAJOR_VERSION}.${_YPSPUR_MINOR_VERSION}.${_YPSPUR_PATCH_VERSION})

  message(WARNING "[deprecated] CMake 2 will be obsoleted in yp-spur build at April, 2019")
else()
  cmake_policy(VERSION 3.0)
  project(ypspur VERSION ${_YPSPUR_MAJOR_VERSION}.${_YPSPUR_MINOR_VERSION}.${_YPSPUR_PATCH_VERSION})
endif()

set(CVF_VERSION ${PROJECT_VERSION})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

set(YP_VENDOR_NAME "OpenSpur.org")
set(YP_PRODUCT_NAME "Yamabico Project - Spur")
set(YP_PROTOCOL_NAME "YPP:10:04")
set(YP_PARAMS_DIR "robot-params")

option(USE_SSM "Compile with SSM if available." ON)

include(GetGitRevision)
get_git_revision()
set(PROJECT_VERSION_REV "${PROJECT_VERSION} (${GIT_REVISION_SHORT})")


# Find dependencies

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(THREAD_LIBRARIES "")
if(THREADS_HAVE_PTHREAD_ARG)
  set(THREAD_LIBRARIES -pthread ${THREAD_LIBRARIES})
endif(THREADS_HAVE_PTHREAD_ARG)
if(CMAKE_THREAD_LIBS_INIT)
  set(THREAD_LIBRARIES ${CMAKE_THREAD_LIBS_INIT} ${THREAD_LIBRARIES})
endif(CMAKE_THREAD_LIBS_INIT)

find_package(PkgConfig)
if(PKGCONFIG_FOUND)
  set(HAVE_PKG_CONFIG 1)
endif(PKGCONFIG_FOUND)

if(USE_SSM)
  find_package(SSM)
endif(USE_SSM)

find_package(Readline)
if(READLINE_FOUND)
  add_definitions(-DHAVE_LIBREADLINE=1)
endif(READLINE_FOUND)

include(CheckFunctionExists)
check_function_exists(longjmp HAVE_LONGJMP)
check_function_exists(siglongjmp HAVE_SIGLONGJMP)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/include/config.h
)
add_definitions(-DHAVE_CONFIG_H=1)

find_package(GTest)


# Set include dirs

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/auxlib
  ${CMAKE_CURRENT_SOURCE_DIR}/auxlib/formula-calc/include
)
if(SSM_FOUND)
  include_directories(
    ${SSM_INCLUDE_DIRS}
  )
endif(SSM_FOUND)


# Static link options

option(ENABLE_ALL_STATIC "Static link libgcc and libstdc++." OFF)
set(TARGET_STATIC_POSTFIX "")
if(ENABLE_ALL_STATIC)
  set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc")
  set(CMAKE_SHARED_LINKER_FLAGS "-static-libgcc")
  set(TARGET_STATIC_POSTFIX "-static")
endif(ENABLE_ALL_STATIC)


# Add subdirectories

add_subdirectory(auxlib)
add_subdirectory(samples)


# Enable gtest

if(GTEST_FOUND)
  enable_testing()

  add_executable(param_load_test
    test/param_load.cpp
    src/adinput.c
    src/communication.c
    src/odometry.c
    src/param.c
    src/serial.c
    src/ssm_spur_handler.c
    src/utility.c
    src/yprintf.c
  )
  target_link_libraries(param_load_test
    ${GTEST_LIBRARIES}
    ${THREAD_LIBRARIES}
    carte2d${TARGET_STATIC_POSTFIX}
    formula-calc${TARGET_STATIC_POSTFIX}
  )

  add_test(YPSpurParamTest param_load_test)
endif(GTEST_FOUND)


# Build coordinator and utilities

add_executable(ypspur-coordinator
  src/adinput.c
  src/command_aux.c
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
  carte2d${TARGET_STATIC_POSTFIX} formula-calc${TARGET_STATIC_POSTFIX} m ${THREAD_LIBRARIES}
)
if(SSM_FOUND)
  target_link_libraries(ypspur-coordinator
    odomssm${TARGET_STATIC_POSTFIX}
    ${SSM_LIBRARIES}
  )
endif(SSM_FOUND)
if(WIN32)
  target_link_libraries(ypspur-coordinator wsock32 ws2_32)
endif(WIN32)

add_executable(ypspur-free
  src/ypspur-free.c
)
target_link_libraries(ypspur-free
  ypspur${TARGET_STATIC_POSTFIX}
)

add_executable(ypspur-interpreter
  src/ypspur-interpreter.c
)

target_link_libraries(ypspur-interpreter
  ypspur${TARGET_STATIC_POSTFIX}
  ${READLINE_LIBRARIES}
)


# Build libraries

set(YPSPUR_LIBRARY_VERSION 14.2.0)
set(YPSPUR_LIBRARY_SOVERSION 14)
set(YPSPUR_PUBLIC_HEADERS
  include/ypspur.h
  include/ypparam.h
)
set(YPSPUR_MD_PUBLIC_HEADERS
  include/ypspur-md.h
  include/ypparam.h
)
add_library(ypspur SHARED
  src/libypspur.c
  src/libypspur-md.c
  src/ipcommunication.c
  src/msq.win32.c
  src/yprintf.c
  src/utility.c
)

add_library(ypspur-comm SHARED
  src/communication.c
)

if(WIN32)
  target_link_libraries(ypspur wsock32 ws2_32)
endif(WIN32)
set_target_properties(ypspur PROPERTIES
  IMPORT_SUFFIX "_import.lib"
  VERSION ${YPSPUR_LIBRARY_VERSION}
  SOVERSION ${YPSPUR_LIBRARY_SOVERSION}
  PUBLIC_HEADER "${YPSPUR_PUBLIC_HEADERS}"
)

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
set_target_properties(ypspur-static PROPERTIES
  OUTPUT_NAME "ypspur"
  PUBLIC_HEADER "${YPSPUR_PUBLIC_HEADERS}"
)

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
set_target_properties(ypspur-md PROPERTIES
  IMPORT_SUFFIX "_import.lib"
  VERSION ${YPSPUR_LIBRARY_VERSION}
  SOVERSION ${YPSPUR_LIBRARY_SOVERSION}
  PUBLIC_HEADER "${YPSPUR_MD_PUBLIC_HEADERS}"
)

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
set_target_properties(ypspur-md-static PROPERTIES
  OUTPUT_NAME "ypspur-md"
  PUBLIC_HEADER "${YPSPUR_MD_PUBLIC_HEADERS}"
)


# Install

install(TARGETS
    ypspur-coordinator
    ypspur
    ypspur-comm
    ypspur-static
    ypspur-md
    ypspur-md-static
    ypspur-free
    ypspur-interpreter
  EXPORT YPSpurExport
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  PUBLIC_HEADER DESTINATION include
)
install(FILES include/yp-spur.h
  DESTINATION include
)
install(FILES include/communication.h
  DESTINATION include/ypspur
)

if(SSM_FOUND)
  install(DIRECTORY
      include/ssmtype
    DESTINATION include
  )
endif(SSM_FOUND)


# Generate package config and install

install(EXPORT YPSpurExport
  DESTINATION share/ypspur/cmake/
  FILE YPSpurTargets.cmake
)

set(YPSPUR_INCLUDE_DIRS include)
set(YPSPUR_CMAKE_DIR share/ypspur/cmake)

include(CMakePackageConfigHelpers)
configure_package_config_file(cmake/ypspur-config.cmake.in
  ${PROJECT_BINARY_DIR}/cmake/ypspur/ypspur-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/share/ypspur
  PATH_VARS YPSPUR_CMAKE_DIR YPSPUR_INCLUDE_DIRS
)
write_basic_package_version_file(
  ${PROJECT_BINARY_DIR}/cmake/ypspur/ypspur-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES
    ${PROJECT_BINARY_DIR}/cmake/ypspur/ypspur-config.cmake
    ${PROJECT_BINARY_DIR}/cmake/ypspur/ypspur-config-version.cmake
    package.xml
  DESTINATION share/ypspur/
)


# Generate pkgconfig file and install

configure_file(ypspur.pc.in
  ${PROJECT_BINARY_DIR}/ypspur.pc @ONLY
)

set(
  PKGCONFIG_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/share/pkgconfig
  CACHE PATH "Install directory of pkgconfig file"
)
install(
  FILES
    ${PROJECT_BINARY_DIR}/ypspur.pc
  DESTINATION ${PKGCONFIG_INSTALL_DIR}
)
