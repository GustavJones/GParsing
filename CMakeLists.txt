cmake_minimum_required(VERSION 3.5)
project(GParsing)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(BUILD_TESTS ON)

message(STATUS "------------------------------")
message(STATUS "BUILDING GParsing IN ${CMAKE_BUILD_TYPE} MODE")
message(STATUS "------------------------------")

add_subdirectory(src)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()

