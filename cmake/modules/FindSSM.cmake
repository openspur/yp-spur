# Find SSM library

find_path(SSM_INCLUDE_DIRS ssm.h
	HINTS ENV SSM_DIR
	PATH_SUFFIXES include
	PATHS /usr /usr/local
)
find_library(SSM_LIBRARIES
	NAMES ssm
	HINTS ENV SSM_DIR
	PATH_SUFFIXES lib lib64
	PATHS /usr /usr/local
)
find_program(SSM_EXECUTABLE
	NAMES ssm-coordinator
	PATHS /usr/bin /usr/local/bin
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SSM
	DEFAULT_MSG SSM_LIBRARIES
	SSM_INCLUDE_DIRS
	SSM_EXECUTABLE
)

