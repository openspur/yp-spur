# Get current git revision (hash) of the repository

function(get_git_revision)

  find_program(GIT
    NAMES git
  )

  if(NOT GIT)
    set(GIT_REVISION_SHORT "Snapshot" PARENT_SCOPE)
    set(GIT_REVISION "Failed to get git revision." PARENT_SCOPE)
  else()
    execute_process(
      COMMAND ${GIT} -C "${CMAKE_CURRENT_SOURCE_DIR}" rev-parse HEAD
      OUTPUT_VARIABLE REVISION
      RESULT_VARIABLE rev_parse_ret
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    execute_process(
      COMMAND ${GIT} -C "${CMAKE_CURRENT_SOURCE_DIR}" rev-parse --short HEAD
      OUTPUT_VARIABLE REVISION_SHORT
      RESULT_VARIABLE rev_parse_short_ret
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if((NOT ${rev_parse_ret} EQUAL "0") OR (NOT ${rev_parse_short_ret} EQUAL "0"))
      set(GIT_REVISION_SHORT "Snapshot" PARENT_SCOPE)
      set(GIT_REVISION "Failed to get git revision." PARENT_SCOPE)
    else()
      set(GIT_REVISION_SHORT ${REVISION_SHORT} PARENT_SCOPE)
      set(GIT_REVISION ${REVISION} PARENT_SCOPE)
    endif()
  endif()

endfunction(get_git_revision)
