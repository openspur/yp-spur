cmake_minimum_required(VERSION 3.8)

# Project version is automatically updated by the release candidate workflow
project(ypspur VERSION 1.23.3)

set(CVF_VERSION ${PROJECT_VERSION})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

set(YP_VENDOR_NAME "OpenSpur.org")
set(YP_PRODUCT_NAME "Yamabico Project - Spur")
set(YP_PROTOCOL_NAME "YPP:11:05")
set(YP_PARAMS_DIR "robot-params")

option(USE_SSM "Compile with SSM if available." ON)

include(GetGitRevision)
get_git_revision()
set(PROJECT_VERSION_REV "${PROJECT_VERSION} (${GIT_REVISION_SHORT})")
message("project version and revision: ${PROJECT_VERSION} (${GIT_REVISION_SHORT})")


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
if(Readline_FOUND)
  add_definitions(-DHAVE_LIBREADLINE=1)
endif(Readline_FOUND)

include(CheckFunctionExists)
check_function_exists(longjmp HAVE_LONGJMP)
check_function_exists(siglongjmp HAVE_SIGLONGJMP)
check_function_exists(clock_nanosleep HAVE_CLOCK_NANOSLEEP)
check_function_exists(nanosleep HAVE_NANOSLEEP)

find_package(GTest)


# Set include dirs

include_directories(
  include
  auxlib/include
  auxlib/formula-calc/include
)
if(SSM_FOUND)
  include_directories(
    ${SSM_INCLUDE_DIRS}
  )
endif(SSM_FOUND)
if(Readline_FOUND)
  include_directories(
    ${Readline_INCLUDE_DIRS}
  )
endif(Readline_FOUND)


# Static link options

option(ENABLE_ALL_STATIC "Static link libgcc and libstdc++." OFF)
set(TARGET_STATIC_POSTFIX "")
if(ENABLE_ALL_STATIC)
  set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc")
  set(CMAKE_SHARED_LINKER_FLAGS "-static-libgcc")
  set(TARGET_STATIC_POSTFIX "-static")
endif(ENABLE_ALL_STATIC)


# Compile flags

if(CMAKE_VERSION VERSION_LESS "3.12")
  add_definitions(
    -DPROJECT_VERSION=\"${PROJECT_VERSION_REV}\"
    -DYP_PROTOCOL_NAME=\"${YP_PROTOCOL_NAME}\"
    -DYP_PARAMS_DIR=\"${YP_PARAMS_DIR}\"
  )

  if(HAVE_LONGJMP)
    add_definitions(-DHAVE_LONGJMP)
  endif()
  if(HAVE_SIGLONGJMP)
    add_definitions(-DHAVE_SIGLONGJMP)
  endif()
  if(HAVE_CLOCK_NANOSLEEP)
    add_definitions(-DHAVE_CLOCK_NANOSLEEP)
  endif()
  if(HAVE_NANOSLEEP)
    add_definitions(-DHAVE_NANOSLEEP)
  endif()
  if(SSM_FOUND)
    add_definitions(-DHAVE_SSM)
  endif()
else()
  add_compile_definitions(
    PROJECT_VERSION=\"${PROJECT_VERSION_REV}\"
    YP_PROTOCOL_NAME=\"${YP_PROTOCOL_NAME}\"
    YP_PARAMS_DIR=\"${YP_PARAMS_DIR}\"
  )

  if(HAVE_LONGJMP)
    add_compile_definitions(HAVE_LONGJMP)
  endif()
  if(HAVE_SIGLONGJMP)
    add_compile_definitions(HAVE_SIGLONGJMP)
  endif()
  if(HAVE_CLOCK_NANOSLEEP)
    add_compile_definitions(HAVE_CLOCK_NANOSLEEP)
  endif()
  if(HAVE_NANOSLEEP)
    add_compile_definitions(HAVE_NANOSLEEP)
  endif()
  if(SSM_FOUND)
    add_compile_definitions(HAVE_SSM)
  endif()
endif()


# Link libraries

set(WIN32_LIBRARIES "")
if(WIN32)
  set(WIN32_LIBRARIES wsock32 ws2_32)
endif(WIN32)


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

  add_executable(e2e_test
    test/e2e.cpp
  )
  add_dependencies(e2e_test
    ypspur-coordinator
    ypspur
  )
  target_link_libraries(e2e_test
    ${GTEST_LIBRARIES}
    ${THREAD_LIBRARIES}
    ypspur
  )
  set_property(TARGET e2e_test PROPERTY CXX_STANDARD 11)

  add_test(E2ETest e2e_test ${CMAKE_SOURCE_DIR}/test/)
endif(GTEST_FOUND)


# Build coordinator library

set(YPSPUR_COORDINATOR_LIBRARY_VERSION 1.0.0)
set(YPSPUR_COORDINATOR_LIBRARY_SOVERSION 1)
set(YPSPUR_COORDINATOR_SOURCES
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
  src/ping.c
  src/serial.c
  src/signal.c
  src/ssm_spur_handler.c
  src/utility.c
  src/ypprotocol.c
  src/yprintf.c
  src/libypspur-coordinator.c
)
set(YPSPUR_COORDINATOR_PUBLIC_HEADERS
  include/ypspur/error_type.h
  include/ypspur/odometry_type.h
  include/ypspur/ypparam.h
  include/ypspur/ypspur-coordinator.h
)

set(YPSPUR_COORDINATOR_SSM_LIBRARIES "")
if(SSM_FOUND)
  set(YPSPUR_COORDINATOR_SSM_LIBRARIES 
    odomssm${TARGET_STATIC_POSTFIX}
    ${SSM_LIBRARIES}
  )
endif(SSM_FOUND)

set(YPSPUR_COORDINATOR_LIBRARIES
  carte2d${TARGET_STATIC_POSTFIX}
  formula-calc${TARGET_STATIC_POSTFIX}
  m
  ${THREAD_LIBRARIES}
  ${WIN32_LIBRARIES}
  ${YPSPUR_COORDINATOR_SSM_LIBRARIES}
)

add_library(libypspur-coordinator SHARED ${YPSPUR_COORDINATOR_SOURCES})
target_link_libraries(libypspur-coordinator ${YPSPUR_COORDINATOR_LIBRARIES})
set_target_properties(libypspur-coordinator PROPERTIES
  OUTPUT_NAME "ypspur-coordinator"
  IMPORT_SUFFIX "_import.lib"
  VERSION ${YPSPUR_COORDINATOR_LIBRARY_VERSION}
  SOVERSION ${YPSPUR_COORDINATOR_LIBRARY_SOVERSION}
  PUBLIC_HEADER "${YPSPUR_COORDINATOR_PUBLIC_HEADERS}"
)

add_library(libypspur-coordinator-static STATIC ${YPSPUR_COORDINATOR_SOURCES})
target_link_libraries(libypspur-coordinator-static ${YPSPUR_COORDINATOR_LIBRARIES})
set_target_properties(libypspur-coordinator-static PROPERTIES
  OUTPUT_NAME "ypspur-coordinator"
  PUBLIC_HEADER "${YPSPUR_COORDINATOR_PUBLIC_HEADERS}"
)


# Build coordinator and utilities

add_executable(ypspur-coordinator
  src/ypspur-coordinator.c
)
target_link_libraries(ypspur-coordinator
  libypspur-coordinator${TARGET_STATIC_POSTFIX}
)

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
  ${Readline_LIBRARIES}
)


# Build libraries

set(YPSPUR_LIBRARY_VERSION 14.2.0)
set(YPSPUR_LIBRARY_SOVERSION 14)

set(YPSPUR_GLOBAL_HEADERS
  include/yp-spur.h
  include/ypspur-md.h
  include/ypspur.h
)

set(YPSPUR_SOURCES
  src/libypspur.c
  src/libypspur-md.c
  src/ipcommunication.c
  src/msq.win32.c
  src/yprintf.c
  src/utility.c
)
set(YPSPUR_PUBLIC_HEADERS
  include/ypspur/ypparam.h
)

set(YPSPUR_MD_SOURCES
  src/libypspur-md.c
  src/ipcommunication.c
  src/msq.win32.c
  src/yprintf.c
  src/utility.c
)
set(YPSPUR_MD_PUBLIC_HEADERS
  include/ypspur/ypparam.h
)

add_library(ypspur SHARED ${YPSPUR_SOURCES})
target_link_libraries(ypspur ${WIN32_LIBRARIES})
set_target_properties(ypspur PROPERTIES
  IMPORT_SUFFIX "_import.lib"
  VERSION ${YPSPUR_LIBRARY_VERSION}
  SOVERSION ${YPSPUR_LIBRARY_SOVERSION}
  PUBLIC_HEADER "${YPSPUR_PUBLIC_HEADERS}"
)

add_library(ypspur-static STATIC ${YPSPUR_SOURCES})
target_link_libraries(ypspur-static ${WIN32_LIBRARIES})
set_target_properties(ypspur-static PROPERTIES
  OUTPUT_NAME "ypspur"
  PUBLIC_HEADER "${YPSPUR_PUBLIC_HEADERS}"
)

add_library(ypspur-md SHARED ${YPSPUR_MD_SOURCES})
target_link_libraries(ypspur-md ${WIN32_LIBRARIES})
set_target_properties(ypspur-md PROPERTIES
  IMPORT_SUFFIX "_import.lib"
  VERSION ${YPSPUR_LIBRARY_VERSION}
  SOVERSION ${YPSPUR_LIBRARY_SOVERSION}
  PUBLIC_HEADER "${YPSPUR_MD_PUBLIC_HEADERS}"
)

add_library(ypspur-md-static STATIC ${YPSPUR_MD_SOURCES})
target_link_libraries(ypspur-md-static ${WIN32_LIBRARIES})
set_target_properties(ypspur-md-static PROPERTIES
  OUTPUT_NAME "ypspur-md"
  PUBLIC_HEADER "${YPSPUR_MD_PUBLIC_HEADERS}"
)

add_library(ypspur-comm SHARED
  src/communication.c
)
set_target_properties(ypspur-comm PROPERTIES
  PUBLIC_HEADER include/ypspur/communication.h
)


# Install

install(TARGETS
    libypspur-coordinator
    libypspur-coordinator-static
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
  PUBLIC_HEADER DESTINATION include/ypspur
)
install(FILES ${YPSPUR_GLOBAL_HEADERS}
  DESTINATION include
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
