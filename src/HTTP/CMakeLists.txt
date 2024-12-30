cmake_minimum_required(VERSION 3.15)
project(GParsing-HTTP)

file(GLOB SOURCES "*.cpp")

if(BUILD_SHARED_LIBS)
  message(STATUS "BUILDING GParsing-HTTP SHARED LIBRARY")
  add_library(${PROJECT_NAME} SHARED ${SOURCES})
  target_compile_definitions(${PROJECT_NAME} PRIVATE GPARSING_SHARED)
else()
  message(STATUS "BUILDING GParsing-HTTP STATIC LIBRARY")
  add_library(${PROJECT_NAME} STATIC ${SOURCES})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ../../include/)
target_compile_definitions(${PROJECT_NAME} PRIVATE GPARSING_BUILD_DLL)
