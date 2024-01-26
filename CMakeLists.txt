cmake_minimum_required(VERSION 3.5)
project(GParsing)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

if(MSVC)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
endif()

file(GLOB SOURCES "src/HTTP/*.cpp" "include/*.hpp")

set(BUILD_SHARED_LIBS ON)
set(BUILD_TESTS OFF)

if(BUILD_SHARED_LIBS)
  add_library(${PROJECT_NAME} SHARED ${SOURCES})
  target_compile_definitions(${PROJECT_NAME} PRIVATE GPARSING_BUILD_DLL)
else()
  add_library(${PROJECT_NAME} STATIC ${SOURCES})
endif()

if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PRIVATE GPARSING_WINDOWS)

elseif(UNIX)
  target_compile_definitions(${PROJECT_NAME} PRIVATE GPARSING_UNIX)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE include)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()
