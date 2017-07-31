# Get current git revision (hash) of the repository

function(get_git_revision)

	find_program(GIT
		NAMES git
	)

	if(NOT GIT)
		set(GIT_REVISION_SHORT "Snapshot" PARENT_SCOPE)
		set(GIT_REVISION "Failed to get git revision." PARENT_SCOPE)
	endif(NOT GIT)

	exec_program(${GIT} ${CMAKE_CURRENT_SOURCE_DIR}
		ARGS "rev-parse HEAD"
		OUTPUT_VARIABLE REVISION
	)
	exec_program(${GIT} ${CMAKE_CURRENT_SOURCE_DIR}
		ARGS "rev-parse --short HEAD"
		OUTPUT_VARIABLE REVISION_SHORT
	)

	set(GIT_REVISION_SHORT ${REVISION_SHORT} PARENT_SCOPE)
	set(GIT_REVISION ${REVISION} PARENT_SCOPE)

endfunction(get_git_revision)

