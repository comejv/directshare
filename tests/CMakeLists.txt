include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/lib ${cmock_SOURCE_DIR}/src)

function(add_unit_test test_name)
  add_executable(${test_name} ${test_name}.c ${cmock_SOURCE_DIR}/src/cmock.c)
  target_link_libraries(${test_name} unity directshare_lib)
  add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_unit_test(test_sample)
