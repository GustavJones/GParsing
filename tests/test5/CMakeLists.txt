cmake_minimum_required(VERSION 3.15)
project(GParsing-Tests-test5)

file(GLOB SOURCES "*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ../../include/)

file(COPY test5.JPG DESTINATION ${CMAKE_CURRENT_BINARY_DIR})