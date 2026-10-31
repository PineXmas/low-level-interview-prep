cmake_minimum_required(VERSION 3.16)

set(BUILD_GMOCK OFF CACHE BOOL "Disable GoogleMock" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "Do not install vendored GoogleTest" FORCE)

add_subdirectory(third_party/googletest)
