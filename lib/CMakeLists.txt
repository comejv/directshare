# Library source files
set(LIB_SOURCES
    # Add your library source files as you create them
    # network_discovery.c
    # connection_manager.c
    # file_transfer.c
    lib_test.c
)

# Create library
add_library(directshare_lib STATIC ${LIB_SOURCES})

# Enable compile commands for this target
set_target_properties(directshare_lib PROPERTIES EXPORT_COMPILE_COMMANDS ON)
