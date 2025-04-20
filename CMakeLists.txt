cmake_minimum_required(VERSION 3.20)
project(DirectShare)

# Options
option(BUILD_TESTING "Build the testing tree" ON)

if(NOT CMAKE_BUILD_TYPE)
  set(default_build_type "Debug")
  message(
    STATUS
      "Set the build type to `${default_build_type}` as none was specified.")
  set(CMAKE_BUILD_TYPE
      ${default_build_type}
      CACHE STRING "Choose the build type." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
                                               "MinSizeRel" "RelWithDebInfo")
endif()

# Turn off global compile commands export
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)

message(STATUS "DirectShare Build Type: ${CMAKE_BUILD_TYPE}")

# Set the version for DirectShare
set(DirectShare_Version_Major 0)
set(DirectShare_Version_Minor 1)
set(DirectShare_Version_Patch 0)
set(DirectShare_Version_Status "-dev")
set(PROJECT_VERSION
    "${DirectShare_Version_Major}.${DirectShare_Version_Minor}.${DirectShare_Version_Patch}${DirectShare_Version_Status}"
)
message(STATUS "${PROJECT_NAME} version: ${PROJECT_VERSION}")

# Force static linking
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)

# Find required packages
find_package(PkgConfig REQUIRED)

# Find libmnl instead of libnl
pkg_check_modules(LIBMNL REQUIRED libmnl)

# For static linking, we need to include all dependencies
set(LIBMNL_LIBRARIES ${LIBMNL_STATIC_LIBRARIES})
set(LIBMNL_LDFLAGS ${LIBMNL_STATIC_LDFLAGS})

# Find static ncurses
find_library(CURSES_LIBRARY NAMES libncursesw.a ncursesw.a libncurses.a
                                  ncurses.a)
find_path(CURSES_INCLUDE_PATH ncurses.h)

if(NOT CURSES_LIBRARY OR NOT CURSES_INCLUDE_PATH)
  message(FATAL_ERROR "Could not find static ncurses library")
endif()

set(CURSES_LIBRARIES ${CURSES_LIBRARY})
set(CURSES_INCLUDE_DIR ${CURSES_INCLUDE_PATH})

# Set linker flags for static linking
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -pthread")

# Set compiler flags
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")
else()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  # Set a CMake variable first
  set(FDEBUG
      0
      CACHE STRING "Debug feature flag")
  # Allow override via command line
  if(DEFINED DEBUG_FEATURE)
    set(FDEBUG
        ${DEBUG_FEATURE}
        CACHE STRING "Debug feature flag" FORCE)
  endif()
  add_compile_definitions(FDEBUG=${FDEBUG})
  message(STATUS "Debug mode: FDEBUG=${FDEBUG}")
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include ${LIBMNL_INCLUDE_DIRS}
                    ${CURSES_INCLUDE_DIR})

# Add subdirectories based on target group
add_subdirectory(src)
add_subdirectory(lib)

if(BUILD_TESTING)
  # Store original flags
  set(ORIGINAL_C_FLAGS "${CMAKE_C_FLAGS}")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")

  # Enable testing
  include(CTest)
  enable_testing()
  include(FetchContent)
  FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.6.1)
  FetchContent_Declare(
    cmock
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/CMock.git
    GIT_TAG v2.6.0)
  FetchContent_MakeAvailable(unity cmock)

  # Add test subdirectory
  add_subdirectory(tests)

  # Restore original flags for your own tests
  set(CMAKE_C_FLAGS "${ORIGINAL_C_FLAGS}")
endif()

# Set install rules
install(TARGETS directshare RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
