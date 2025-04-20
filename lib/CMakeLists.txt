# Library source files
set(LIB_SOURCES net_discovery.c)

# Create library
add_library(directshare_lib STATIC ${LIB_SOURCES})

# Enable compile commands for this target
set_target_properties(directshare_lib PROPERTIES EXPORT_COMPILE_COMMANDS ON)

target_link_libraries(directshare_lib ${LIBMNL_LIBRARIES})
