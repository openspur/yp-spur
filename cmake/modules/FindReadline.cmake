# Find readline library

find_path(Readline_INCLUDE_DIRS readline/readline.h
	HINTS ENV READLINE_DIR
	PATH_SUFFIXES include
)
find_library(Readline_LIBRARIES_FOUND
	NAMES readline
	HINTS ENV READLINE_DIR
)
if(Readline_LIBRARIES_FOUND)
	set(Readline_LIBRARIES readline)
endif(Readline_LIBRARIES_FOUND)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Readline
	DEFAULT_MSG
  Readline_LIBRARIES
	Readline_INCLUDE_DIRS
)
