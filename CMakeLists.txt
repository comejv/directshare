cmake_minimum_required(VERSION 3.20)
project(DirectShare VERSION 0.1.0)

# ------------------------------------------------------------------------------
# Build Options
# ------------------------------------------------------------------------------
option(BUILD_TESTING "Build the testing tree" ON)

if(NOT CMAKE_BUILD_TYPE)
  set(default_build_type "Debug")
  message(
    STATUS
      "No build type specified. Setting CMAKE_BUILD_TYPE to ${default_build_type}."
  )
  set(CMAKE_BUILD_TYPE
      ${default_build_type}
      CACHE STRING "Choose the build type." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
                                               "MinSizeRel" "RelWithDebInfo")
endif()

# ------------------------------------------------------------------------------
# Global Settings
# ------------------------------------------------------------------------------
# Turn off global compile commands export
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)
message(STATUS "DirectShare Build Type: ${CMAKE_BUILD_TYPE}")

# Force static linking
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -pthread")

# Set C standard and compiler flags
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")
else()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
endif()

# Optional debug macro if in Debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(FDEBUG
      0
      CACHE STRING "Debug feature flag")
  if(DEFINED DEBUG_FEATURE)
    set(FDEBUG
        ${DEBUG_FEATURE}
        CACHE STRING "Debug feature flag" FORCE)
  endif()
  add_compile_definitions(FDEBUG=${FDEBUG})
  message(STATUS "Debug mode: FDEBUG=${FDEBUG}")
endif()

# ------------------------------------------------------------------------------
# External Libraries and Dependencies
# ------------------------------------------------------------------------------
# Find pkg-config
find_package(PkgConfig REQUIRED)

# --- libmnl (using static libraries) ---
pkg_check_modules(LIBMNL REQUIRED libmnl)
set(LIBMNL_LIBRARIES ${LIBMNL_STATIC_LIBRARIES})
set(LIBMNL_LDFLAGS ${LIBMNL_STATIC_LDFLAGS})

# --- ncurses (using static libraries) ---
find_library(CURSES_LIBRARY NAMES libncursesw.a ncursesw.a libncurses.a
                                  ncurses.a)
find_path(CURSES_INCLUDE_PATH ncurses.h)
if(NOT CURSES_LIBRARY OR NOT CURSES_INCLUDE_PATH)
  message(FATAL_ERROR "Could not find static ncurses library")
endif()
set(CURSES_LIBRARIES ${CURSES_LIBRARY})
set(CURSES_INCLUDE_DIR ${CURSES_INCLUDE_PATH})

# ------------------------------------------------------------------------------
# Project Subdirectories
# ------------------------------------------------------------------------------
add_subdirectory(src)
add_subdirectory(lib)

# ------------------------------------------------------------------------------
# Testing (Optional)
# ------------------------------------------------------------------------------
# Uncomment and adjust the following section to enable testing.
#
# if(BUILD_TESTING) # Save original flags and enforce O2 optimization for
# testing set(ORIGINAL_C_FLAGS "${CMAKE_C_FLAGS}") set(CMAKE_C_FLAGS
# "${CMAKE_C_FLAGS} -O2")
#
# include(CTest) enable_testing()
#
# include(FetchContent) FetchContent_Declare( unity GIT_REPOSITORY
# https://github.com/ThrowTheSwitch/Unity.git GIT_TAG v2.6.1 )
# FetchContent_MakeAvailable(unity)
#
# add_subdirectory(tests) # Restore original flags set(CMAKE_C_FLAGS
# "${ORIGINAL_C_FLAGS}") endif()

# ------------------------------------------------------------------------------
# Install Rules
# ------------------------------------------------------------------------------
# Installs the executable (adjust destination as needed)
install(TARGETS directshare RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
