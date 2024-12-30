cmake_minimum_required(VERSION 3.15)
project(GParsing-Tests-test2)

file(GLOB SOURCES "*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ../../include/)
target_link_libraries(${PROJECT_NAME} PRIVATE GParsing-HTTP)
