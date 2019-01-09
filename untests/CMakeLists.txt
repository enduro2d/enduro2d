function(add_e2d_tests NAME)
    set(TESTS_NAME untests_${NAME})

    #
    # sources
    #

    file(GLOB ${TESTS_NAME}_sources
        sources/*.*
        sources/${TESTS_NAME}/*.*)
    set(TESTS_SOURCES ${${TESTS_NAME}_sources})
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TESTS_SOURCES})

    #
    # executable
    #

    add_executable(${TESTS_NAME}
        ${TESTS_SOURCES})
    target_link_libraries(${TESTS_NAME}
        ${E2D_LIBRARIES})
    target_include_directories(${TESTS_NAME}
        PRIVATE ${E2D_INCLUDE_DIRS})
    set_target_properties(${TESTS_NAME} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

    #
    # resources
    #

    add_custom_command(TARGET ${TESTS_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/bin
        $<TARGET_FILE_DIR:${TESTS_NAME}>/bin)

    #
    # tests
    #

    add_test(${TESTS_NAME} ${TESTS_NAME})
endfunction(add_e2d_tests)

add_e2d_tests(base)
add_e2d_tests(core)
add_e2d_tests(high)
add_e2d_tests(math)
add_e2d_tests(utils)
