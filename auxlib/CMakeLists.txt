cmake_minimum_required(VERSION 2.8.3)

set(CARTE2D_LIBRARY_VERSION 0.0.0)
set(CARTE2D_LIBRARY_SOVERSION 0)
add_library(carte2d SHARED libcarte2d.c)
target_link_libraries(carte2d m)
set_target_properties(carte2d PROPERTIES
  IMPORT_SUFFIX "_import.lib"
  VERSION ${CARTE2D_LIBRARY_VERSION}
  SOVERSION ${CARTE2D_LIBRARY_SOVERSION}
  PUBLIC_HEADER cartesian2d.h
)

add_library(carte2d-static STATIC libcarte2d.c)
target_link_libraries(carte2d-static m)
set_target_properties(carte2d-static PROPERTIES
  OUTPUT_NAME "carte2d"
  PUBLIC_HEADER cartesian2d.h
)

install(TARGETS
    carte2d
    carte2d-static
  EXPORT YPSpurExport
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  PUBLIC_HEADER DESTINATION include
)

if(SSM_FOUND)
  set(ODOMSSM_LIBRARY_VERSION 0.0.1)
  set(ODOMSSM_LIBRARY_SOVERSION 0)
  add_library(odomssm SHARED libodomssm.c)
  target_link_libraries(odomssm ${SSM_LIBRARIES})
  set_target_properties(odomssm PROPERTIES 
    VERSION ${ODOMSSM_LIBRARY_VERSION}
    SOVERSION ${ODOMSSM_LIBRARY_SOVERSION}
    PUBLIC_HEADER odometry-ssm.h
  )
  set_target_properties(odomssm PROPERTIES IMPORT_SUFFIX "_import.lib")

  add_library(odomssm-static STATIC libodomssm.c)
  target_link_libraries(odomssm-static ${SSM_LIBRARIES})
  set_target_properties(odomssm-static PROPERTIES
    OUTPUT_NAME "odomssm"
    PUBLIC_HEADER odometry-ssm.h
  )

  install(TARGETS
      odomssm
      odomssm-static
    EXPORT YPSpurExport
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include
  )
endif(SSM_FOUND)

add_subdirectory(formula-calc)

