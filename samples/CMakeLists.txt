function(add_e2d_sample NAME)
    set(SAMPLE_NAME sample_${NAME})

    #
    # sources
    #

    file(GLOB ${SAMPLE_NAME}_sources
        sources/*.*
        sources/${SAMPLE_NAME}/*.*)
    set(SAMPLE_SOURCES ${${SAMPLE_NAME}_sources})
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SAMPLE_SOURCES})

    #
    # executable
    #

    add_executable(${SAMPLE_NAME}
        ${SAMPLE_SOURCES})
    target_link_libraries(${SAMPLE_NAME}
        ${E2D_LIBRARIES})
    target_include_directories(${SAMPLE_NAME}
        PRIVATE ${E2D_INCLUDE_DIRS})
    set_target_properties(${SAMPLE_NAME} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

    #
    # resources
    #

    add_custom_command(TARGET ${SAMPLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/bin
        $<TARGET_FILE_DIR:${SAMPLE_NAME}>/bin)
endfunction(add_e2d_sample)

add_e2d_sample(00)
add_e2d_sample(01)
add_e2d_sample(02)
add_e2d_sample(03)
add_e2d_sample(04)
