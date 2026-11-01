# Derive the problem name from the directory containing the including manifest.
get_filename_component(PROBLEM_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
set(PROBLEM_TARGET "prob_${PROBLEM_NAME}")

# Collect the problem's C implementation files while allowing additions to be
# detected automatically on subsequent CMake configuration runs.
file(GLOB PROBLEM_C_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

# Build the C++ test runner together with all local C implementation files.
add_executable("${PROBLEM_TARGET}"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_runner.cpp"
    ${PROBLEM_C_SOURCES}
)

# Make problem-local headers and shared infrastructure support headers visible.
target_include_directories("${PROBLEM_TARGET}" PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${REPOSITORY_ROOT}/infra/support"
)

# Link the standard GoogleTest entry point and register each test case with CTest.
target_link_libraries("${PROBLEM_TARGET}" PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests("${PROBLEM_TARGET}"
    PROPERTIES LABELS problem
)
