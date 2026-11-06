# Derive the problem name from the directory containing the including manifest.
get_filename_component(PROBLEM_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
set(PROBLEM_TARGET "prob_${PROBLEM_NAME}")

# Customize this list with every source that belongs to the problem. Unlike the
# standard template, this template intentionally does not discover sources.
set(PROBLEM_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/test_runner.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/problem_impl.c"
)

# Add problem-local or infrastructure include directories here as needed.
set(PROBLEM_INCLUDE_DIRECTORIES
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${REPOSITORY_ROOT}/infra/support"
)

# Add optional compile definitions and libraries in these lists.
set(PROBLEM_COMPILE_DEFINITIONS)
set(PROBLEM_EXTRA_LIBRARIES)

add_executable("${PROBLEM_TARGET}" ${PROBLEM_SOURCES})

target_include_directories("${PROBLEM_TARGET}" PRIVATE
    ${PROBLEM_INCLUDE_DIRECTORIES}
)

if(PROBLEM_COMPILE_DEFINITIONS)
    target_compile_definitions("${PROBLEM_TARGET}" PRIVATE
        ${PROBLEM_COMPILE_DEFINITIONS}
    )
endif()

target_link_libraries("${PROBLEM_TARGET}" PRIVATE
    gtest_main
    ${PROBLEM_EXTRA_LIBRARIES}
)

include(GoogleTest)
gtest_discover_tests("${PROBLEM_TARGET}"
    TEST_PREFIX "${PROBLEM_NAME}/"
    PROPERTIES LABELS problem
)
