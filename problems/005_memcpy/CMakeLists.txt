set(PROBLEM_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/test_runner.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/my_memcpy.c"
    "${REPOSITORY_ROOT}/infra/support/utils.c"
)

set(PROBLEM_INCLUDE_DIRECTORIES
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${REPOSITORY_ROOT}/infra/support"
)

include("${REPOSITORY_ROOT}/infra/cmake/templates/special-problem.cmake")
