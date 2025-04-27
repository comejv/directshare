# Source files
set(SOURCES main.c)

# Create executable
add_executable(directshare ${SOURCES})

# Enable compile commands for this target
set_target_properties(directshare PROPERTIES EXPORT_COMPILE_COMMANDS ON)

# Include directories
target_include_directories(directshare PRIVATE ${CMAKE_SOURCE_DIR}/include
                                               ${CURSES_INCLUDE_DIR})

# Link libraries
target_link_libraries(directshare PRIVATE directshare_lib ${CURSES_LIBRARIES} m)
