cmake_minimum_required(VERSION 3.15)
project(GParsing-HTTP)

file(GLOB SOURCES "*.cpp")

add_library(${PROJECT_NAME} ${SOURCES})

if(BUILD_SHARED_LIBS)
  message(STATUS "BUILDING GParsing-HTTP SHARED LIBRARY")
  target_compile_definitions(${PROJECT_NAME} PRIVATE GPARSING_SHARED)
else()
  message(STATUS "BUILDING GParsing-HTTP STATIC LIBRARY")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ../../include/)
target_compile_definitions(${PROJECT_NAME} PRIVATE GPARSING_BUILD_DLL)

set_target_properties(${PROJECT_NAME} PROPERTIES 
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED TRUE
)

if(GPARSING_HTTP)
  target_link_libraries(GParsing INTERFACE GParsing-HTTP)
endif()
