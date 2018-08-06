function(add_e2d_sample NAME)
  set(SAMPLE_NAME sample_${NAME})
  file(GLOB ${SAMPLE_NAME}_sources
    sources/*.*
    sources/${SAMPLE_NAME}/*.*)
  set(SAMPLE_SOURCES ${${SAMPLE_NAME}_sources})
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SAMPLE_SOURCES})
  add_executable(${SAMPLE_NAME} ${SAMPLE_SOURCES})
  target_include_directories(${SAMPLE_NAME} PRIVATE "../headers")
  target_link_libraries(${SAMPLE_NAME} enduro2d)
  target_link_libraries(${SAMPLE_NAME} "${CMAKE_THREAD_LIBS_INIT}")
endfunction(add_e2d_sample)

add_e2d_sample(00)
add_e2d_sample(01)
