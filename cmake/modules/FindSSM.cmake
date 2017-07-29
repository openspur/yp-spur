# Find SSM library

find_path(SSM_INCLUDE_DIRS ssm.h
	HINTS ENV SSM_DIR
	PATH_SUFFIXES include
)
find_library(SSM_LIBRARIES
	NAMES ssm
	HINTS ENV SSM_DIR
)
find_program(SSM_EXECUTABLE
	NAMES ssm-coordinator
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SSM
	DEFAULT_MSG SSM_LIBRARIES
	SSM_INCLUDE_DIRS
	SSM_EXECUTABLE
)

