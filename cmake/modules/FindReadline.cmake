# Find readline library

find_path(READLINE_INCLUDE_DIRS readline/readline.h
	HINTS ENV READLINE_DIR
	PATH_SUFFIXES include
)
find_library(READLINE_LIBRARIES_FOUND
	NAMES readline
	HINTS ENV READLINE_DIR
)
if(READLINE_LIBRARIES_FOUND)
	set(READLINE_LIBRARIES readline)
endif(READLINE_LIBRARIES_FOUND)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(READLINE
	DEFAULT_MSG READLINE_LIBRARIES
	READLINE_INCLUDE_DIRS
)
