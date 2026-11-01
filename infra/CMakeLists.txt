cmake_minimum_required(VERSION 3.16)

set(BUILD_GMOCK OFF CACHE BOOL "Disable GoogleMock" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "Do not install vendored GoogleTest" FORCE)

add_subdirectory(third_party/googletest)

add_executable(framework_smoke_test support/framework_smoke_test.cpp)
target_link_libraries(framework_smoke_test PRIVATE gtest_main)

add_test(NAME framework_smoke_test COMMAND framework_smoke_test)
