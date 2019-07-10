cmake_minimum_required(VERSION 3.10)
project(ext
  LANGUAGES C
  VERSION 1.0.0
)

set(CMAKE_C_STANDARD 11)

option(USE_GIT_VERSION "use git to get commit info for version command" ON)

if(${USE_GIT_VERSION})
  find_package(Git REQUIRED)

  # the commit's SHA1, and whether the building workspace was dirty or not
  execute_process(COMMAND
    "${GIT_EXECUTABLE}" describe --match=NeVeRmAtCh --always --abbrev=40 --dirty
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_COMMIT_SHA1
    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  # the date of the commit
  execute_process(COMMAND
    "${GIT_EXECUTABLE}" log -1 --format=%ad --date=local
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_COMMIT_DATE
    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  # the subject of the commit
  execute_process(COMMAND
    "${GIT_EXECUTABLE}" log -1 --format=%s
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_COMMIT_MESSAGE
    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  if ("${GIT_COMMIT_SHA1}" STREQUAL "")
    set(GIT_COMMIT_SHA1 "NULL")
    set(GIT_COMMIT_DATE "NULL")
    set(GIT_COMMIT_MESSAGE "NULL")

    message(WARNING "No git commit info found")
  else()
    string(CONFIGURE "\"@GIT_COMMIT_SHA1@\"" GIT_COMMIT_SHA1 ESCAPE_QUOTES)
    string(CONFIGURE "\"@GIT_COMMIT_DATE@\"" GIT_COMMIT_DATE ESCAPE_QUOTES)
    string(CONFIGURE "\"@GIT_COMMIT_MESSAGE@\"" GIT_COMMIT_MESSAGE ESCAPE_QUOTES)
  endif()
else(${USE_GIT_VERSION})
  set(GIT_COMMIT_SHA1 "NULL")
  set(GIT_COMMIT_DATE "NULL")
  set(GIT_COMMIT_MESSAGE "NULL")
endif(${USE_GIT_VERSION})

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/src/version.c.in"
  "${CMAKE_CURRENT_BINARY_DIR}/version.c"
  @ONLY
)

add_executable(ext
  src/main.c
  ${CMAKE_CURRENT_BINARY_DIR}/version.c
)

target_include_directories(ext
  PRIVATE
    src
)

install(
  TARGETS ext
  RUNTIME DESTINATION bin
)
