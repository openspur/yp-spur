cmake_minimum_required(VERSION 3.0.2)

include_directories(./)

add_library(carte2d libcarte2d.c)
set_target_properties(carte2d
	PROPERTIES 
		VERSION 0.0 SOVERSION 0
		PUBLIC_HEADER cartesian2d.h
	)

if(SSM_FOUND)
	add_library(odomssm libodomssm.c)
	set_target_properties(odomssm
		PROPERTIES 
			VERSION 0.0 SOVERSION 0
			PUBLIC_HEADER odometry-ssm.h
		)
endif(SSM_FOUND)

add_subdirectory(formula-calc)

