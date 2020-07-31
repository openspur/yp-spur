# Get current git revision (hash) of the repository

function(get_git_revision)

  find_program(GIT
    NAMES git
  )

  if(NOT GIT)
    set(GIT_REVISION_SHORT "Snapshot" PARENT_SCOPE)
    set(GIT_REVISION "Failed to get git revision." PARENT_SCOPE)
  else()
    exec_program(${GIT} -C "${CMAKE_CURRENT_SOURCE_DIR}"
      ARGS "rev-parse HEAD"
      OUTPUT_VARIABLE REVISION
      RETURN_VALUE rev_parse_ret
    )
    exec_program(${GIT} -C "${CMAKE_CURRENT_SOURCE_DIR}"
      ARGS "rev-parse --short HEAD"
      OUTPUT_VARIABLE REVISION_SHORT
      RETURN_VALUE rev_parse_short_ret
    )

    message(WARNING "${rev_parse_ret} ${rev_parse_short_ret}")
    if((NOT ${rev_parse_ret} EQUAL "0") OR (NOT ${rev_parse_short_ret} EQUAL "0"))
      set(GIT_REVISION_SHORT "Snapshot" PARENT_SCOPE)
      set(GIT_REVISION "Failed to get git revision." PARENT_SCOPE)
    else()
      set(GIT_REVISION_SHORT ${REVISION_SHORT} PARENT_SCOPE)
      set(GIT_REVISION ${REVISION} PARENT_SCOPE)
    endif()
  endif()

endfunction(get_git_revision)
