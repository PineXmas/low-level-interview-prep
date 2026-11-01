cmake_minimum_required(VERSION 3.16)

project(low_level_interview_prep LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(REPOSITORY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

include(CTest)
enable_testing()

add_subdirectory(infra)
