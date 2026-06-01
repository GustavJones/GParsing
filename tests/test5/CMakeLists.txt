cmake_minimum_required(VERSION 3.15)
project(GParsing-Tests-test5)

file(GLOB SOURCES "*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE GParsing)

file(COPY test5.JPG DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
