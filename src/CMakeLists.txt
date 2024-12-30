cmake_minimum_required(VERSION 3.15)
project(GParsing)

add_subdirectory(HTTP)

# if(BUILD_SHARED_LIBS)
#   message(STATUS "BUILDING GParsing SHARED LIBRARY")
#   add_library(${PROJECT_NAME} SHARED ../include/GParsing/Core/API.hpp)
# else()
#   message(STATUS "BUILDING GParsing STATIC LIBRARY")
#   add_library(${PROJECT_NAME} STATIC ../include/GParsing/Core/API.hpp)
# endif()
#
# target_link_libraries(${PROJECT_NAME} PUBLIC GParsing-HTTP)
