# Source files
set(SOURCES main.c)

# Create executable
add_executable(directshare ${SOURCES})

# Enable compile commands for this target
set_target_properties(directshare PROPERTIES EXPORT_COMPILE_COMMANDS ON)

# Link libraries
target_link_libraries(directshare directshare_lib ${CURSES_LIBRARIES})
