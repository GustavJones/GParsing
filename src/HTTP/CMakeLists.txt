cmake_minimum_required(VERSION 3.5)
project(GParsing-HTTP)

file(GLOB SOURCES "*.cpp")
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ../../include/)
target_compile_definitions(${PROJECT_NAME} PRIVATE GPARSING_BUILD_DLL)
