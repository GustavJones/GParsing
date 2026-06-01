cmake_minimum_required(VERSION 3.15)
project(GParsing)

add_library(GParsing INTERFACE)
target_include_directories(GParsing INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(GPARSING_BUILD_TESTS OFF)
option(GPARSING_HTTP "Link HTTP library" ON)

include(GNUInstallDirs)
set(CMAKE_INSTALL_RPATH "$ORIGIN/${CMAKE_INSTALL_LIBDIR} ${CMAKE_INSTALL_FULL_LIBDIR}")
message(STATUS "Using default install RPATH of: ${CMAKE_INSTALL_RPATH}")

file(GLOB SUBMODULES "external/*")

foreach(SUBMODULE ${SUBMODULES})
  if(IS_DIRECTORY ${SUBMODULE})
    if(EXISTS "${SUBMODULE}/CMakeLists.txt")
      message(STATUS "Adding submodule: ${SUBMODULE}")
      add_subdirectory(${SUBMODULE})
    endif()
  endif()
endforeach()

add_subdirectory(src)

if(GPARSING_BUILD_TESTS)
  add_subdirectory(tests)
endif()

