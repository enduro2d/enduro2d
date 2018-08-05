function(add_e2d_tests NAME)
  set(TESTS_NAME untests_${NAME})
  file(GLOB ${TESTS_NAME}_sources
    sources/*.*
	sources/${TESTS_NAME}/*.*)
  set(TESTS_SOURCES ${${TESTS_NAME}_sources})
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TESTS_SOURCES})
  add_executable(${TESTS_NAME} ${TESTS_SOURCES})
  target_include_directories(${TESTS_NAME} PRIVATE "../headers")
  target_link_libraries(${TESTS_NAME} enduro2d)
  add_test(${TESTS_NAME} ${TESTS_NAME})
endfunction(add_e2d_tests)

add_e2d_tests(base)
add_e2d_tests(core)
add_e2d_tests(math)
add_e2d_tests(utils)
