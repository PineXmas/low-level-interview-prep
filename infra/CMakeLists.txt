cmake_minimum_required(VERSION 3.16)

set(BUILD_GMOCK OFF CACHE BOOL "Disable GoogleMock" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "Do not install vendored GoogleTest" FORCE)

add_subdirectory(third_party/googletest)

# Discover problem directories whenever the repository layout changes.
file(GLOB PROBLEM_DIRECTORIES CONFIGURE_DEPENDS
    "${REPOSITORY_ROOT}/problems/*"
)

# Only migrated problems have a CMake manifest and participate in this build.
foreach(PROBLEM_DIRECTORY IN LISTS PROBLEM_DIRECTORIES)
    if(IS_DIRECTORY "${PROBLEM_DIRECTORY}"
       AND EXISTS "${PROBLEM_DIRECTORY}/CMakeLists.txt")
        get_filename_component(PROBLEM_NAME "${PROBLEM_DIRECTORY}" NAME)
        add_subdirectory(
            "${PROBLEM_DIRECTORY}"
            "${CMAKE_CURRENT_BINARY_DIR}/problems/${PROBLEM_NAME}"
        )
    endif()
endforeach()

add_executable(framework_smoke_test support/framework_smoke_test.cpp)
target_link_libraries(framework_smoke_test PRIVATE gtest_main)

add_test(NAME framework_smoke_test COMMAND framework_smoke_test)
